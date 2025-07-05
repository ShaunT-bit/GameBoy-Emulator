#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"


#define MEMORY_SIZE 0X10000

int main(int argc, char *argv[]) {

    if (argc <2)
    {
        printf("Usage: %s Tetris.gb\n",argv[0]);
        return 1;
    }

    FILE *rom = fopen(argv[1],"rb");

    if (!rom)
    {
        perror("could not open ROM");
        return 1;
    }


    uint8_t *memory = calloc(MEMORY_SIZE, sizeof(uint8_t));

    if (!memory)
    {
        perror("Failed to allocate memory");
        return 1;
    }

    fread(memory, 1, 0X8000, rom);
    fclose(rom);

    CPU cpu;

    cpu_init(&cpu);


    for (int i = 0; i < 200; i++)
    {
        cpu_step(&cpu, memory);
    }


    free(memory);
    return 0;
}