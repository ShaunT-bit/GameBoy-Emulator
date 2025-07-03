

#ifndef CPU_H
#define CPU_H


#include <stdint.h>

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10


typedef struct{

    uint16_t pc; // Program counter
    uint16_t sp; // stack Pointer
    uint8_t a, f; //accumulator & flags
    uint8_t b, c;
    uint8_t d, e;
    uint8_t h, l;

    int cycles;


} CPU;

void cpu_init(CPU *cpu);
void cpu_step(CPU *cpu, uint8_t *memory);


#endif //CPU_H
