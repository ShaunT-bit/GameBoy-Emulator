
#ifndef MMU_H
#define MMU_H

#include <stdint.h>


uint8_t mmu_read_byte(uint8_t *memory_array, uint16_t address);

void mmu_write_byte(uint8_t *memory_array, uint16_t address, uint8_t value);

#endif //MMU_H
