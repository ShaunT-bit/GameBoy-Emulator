#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "tinyfiledialogs.h"

#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <ui.h>
#include <timer.h>
#include <dma.h>
#include <ppu.h>

static emu_context ctx;

#define TETRIS_ROM "tetris.gb"

#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 300

typedef struct {
    SDL_Rect rect;
    const char *label;
    bool hover;
} Button;

static char selected_rom[512] = {0};
static TTF_Font *retro_font = NULL;

emu_context *emu_get_context() {
    return &ctx;
}

void *cpu_run(void *p) {
    timer_init();
    cpu_init();
    ppu_init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while (ctx.running) {
        if (ctx.paused) {
            SDL_Delay(10);
            continue;
        }

        if (!cpu_step()) {
            printf("CPU Stopped\n");
            return 0;
        }
    }
    return 0;
}

bool point_in_rect(int x, int y, SDL_Rect rect) {
    return x > rect.x && x < rect.x + rect.w &&
           y > rect.y && y < rect.y + rect.h;
}

void open_file_picker() {
    const char* filters[] = { "*.gb", "*.gbc" };
    const char *rom = tinyfd_openFileDialog(
        "Select a Game Boy ROM",
        "",
        2, filters, NULL, 0);
    if (rom) {
        strncpy(selected_rom, rom, sizeof(selected_rom)-1);
        printf("Selected ROM: %s\n", selected_rom);
    }
}

void draw_text(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(retro_font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int show_menu(SDL_Window *window, SDL_Renderer *renderer) {
    Button buttons[3] = {
        { {100, 60, 200, 50}, "Choose ROM", false },
        { {100, 130, 200, 50}, "Play", false },
        { {100, 200, 200, 50}, "Tetris", false }
    };

    SDL_Color textColor = { 0, 255, 0, 255 }; // retro green
    bool running = true;
    SDL_Event e;

    while (running) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return -1;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < 3; i++) {
                    if (point_in_rect(mx, my, buttons[i].rect)) {
                        if (i == 0) open_file_picker();
                        else if (i == 1 && strlen(selected_rom) > 0) return 1;
                        else if (i == 2) {
                            strncpy(selected_rom, TETRIS_ROM, sizeof(selected_rom)-1);
                            return 1;
                        }
                    }
                }
            }
        }

        // Update hover states
        for (int i = 0; i < 3; i++) {
            buttons[i].hover = point_in_rect(mx, my, buttons[i].rect);
        }

        // Background (retro black with scanline feel)
        SDL_SetRenderDrawColor(renderer, 10, 30, 10, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < 3; i++) {
            // Button fill
            if (buttons[i].hover)
                SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 70, 0, 255);
            SDL_RenderFillRect(renderer, &buttons[i].rect);

            // Border
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &buttons[i].rect);

            // Text
            draw_text(renderer, buttons[i].label,
                      buttons[i].rect.x + 40, buttons[i].rect.y + 15,
                      textColor);
        }

        SDL_RenderPresent(renderer);
    }
    return 0;
}

int emu_run(int argc, char **argv) {
    const char *rom_path = NULL;

    if (argc < 2) {
        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();
        retro_font = TTF_OpenFont("../../NotoSansMono-Medium.ttf", 24);
        if (!retro_font) {
            printf("Failed to load font: %s\n", TTF_GetError());
            return -1;
        }

        SDL_Window *window = SDL_CreateWindow("Game Boy Emulator Menu",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        int menu_result = show_menu(window, renderer);

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(retro_font);
        TTF_Quit();
        SDL_Quit();

        if (menu_result == 1 && strlen(selected_rom) > 0) {
            rom_path = selected_rom;
        } else {
            return -1;
        }
    } else {
        rom_path = argv[1];
    }

    if (!cart_load(rom_path)) {
        printf("Failed to load ROM file: %s\n", rom_path);
        return -2;
    }

    printf("Cart loaded: %s\n", rom_path);

    ui_init();

    pthread_t t1;
    if (pthread_create(&t1, NULL, cpu_run, NULL)) {
        fprintf(stderr, "FAILED TO START MAIN CPU THREAD!\n");
        return -1;
    }

    u32 prev_frame = 0;
    while (!ctx.die) {
        usleep(1000);
        ui_handle_events();
        if (prev_frame != ppu_get_context()->current_frame) {
            ui_update();
        }
        prev_frame = ppu_get_context()->current_frame;
    }

    return 0;
}

void emu_cycles(int cpu_cycles) {
    for (int i = 0; i < cpu_cycles; i++) {
        for (int n = 0; n < 4; n++) {
            ctx.ticks++;
            timer_tick();
            ppu_tick();
        }
        dma_tick();
    }
}
