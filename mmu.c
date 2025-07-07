#include "mmu.h"
#include <stdio.h>


uint8_t mmu_read_byte(uint8_t *memory_array, uint16_t address) {

    if (address >= 0x0000 && address <= 0x7FFF) {
        // ROM Bank 0 and switchable ROM banks (ROM0 and ROMX)
        return memory_array[address];
    } else if (address >= 0x8000 && address <= 0x9FFF) {
        // VRAM (Video RAM) - Used by the PPU for tile data and map data.
        return memory_array[address];
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        // Cartridge RAM (External RAM) - Can be present on cartridges (e.g., for saves).
        return memory_array[address];
    } else if (address >= 0xC000 && address <= 0xDFFF) {
        // WRAM (Work RAM) - Internal CPU RAM.
        return memory_array[address];
    } else if (address >= 0xE000 && address <= 0xFDFF) {
        // Echo RAM - A mirror of WRAM (0xC000-0xDFFF).
        return memory_array[address - 0x2000]; // E000 maps to C000, E001 to C001, etc.
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // OAM (Object Attribute Memory) - For PPU sprites.
        return memory_array[address];
    } else if (address >= 0xFEA0 && address <= 0xFEFF) {
        // Unusable/Forbidden memory range. Reads always return 0xFF.
        return 0xFF;
    } else if (address >= 0xFF00 && address <= 0xFF7F) {
        // I/O Registers - Controls PPU, Timers, Joypad, etc.
        return memory_array[address];
    } else if (address >= 0xFF80 && address <= 0xFFFE) {
        // HRAM (High RAM) - Small, fast RAM.
        return memory_array[address];
    } else if (address == 0xFFFF) {
        // IE (Interrupt Enable) Register.
        return memory_array[address];
    }

    printf("MMU: Attempted to read from unhandled address 0x%04X\n", address);
    return 0xFF;
}

// Define the MMU write function
void mmu_write_byte(uint8_t *memory_array, uint16_t address, uint8_t value) {

    if (address >= 0x0000 && address <= 0x7FFF) {
        // read only
    } else if (address >= 0x8000 && address <= 0x9FFF) {

        memory_array[address] = value;
    } else if (address >= 0xA000 && address <= 0xBFFF) {

        memory_array[address] = value;
    } else if (address >= 0xC000 && address <= 0xDFFF) {

        memory_array[address] = value;
    } else if (address >= 0xE000 && address <= 0xFDFF) {

        memory_array[address - 0x2000] = value;
    } else if (address >= 0xFE00 && address <= 0xFE9F) {

        memory_array[address] = value;
    } else if (address >= 0xFEA0 && address <= 0xFEFF) {
        // printf("MMU: Warning: Attempted to write to forbidden address 0x%04X (value 0x%02X)\n", address, value);
    } else if (address >= 0xFF00 && address <= 0xFF7F) {

        memory_array[address] = value;
    } else if (address >= 0xFF80 && address <= 0xFFFE) {

        memory_array[address] = value;
    } else if (address == 0xFFFF) {

        memory_array[address] = value;
    } else {

        printf("MMU: Attempted to write to unhandled address 0x%04X (value 0x%02X)\n", address, value);
    }
}