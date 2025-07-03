
#include "cpu.h"
#include <stdio.h>

void cpu_init(CPU *cpu)
{
    cpu -> pc = 0x100;
    cpu -> sp = 0xFFFE;

    cpu->a = 0;
    cpu->f = 0;
    cpu->b = 0;
    cpu->c = 0;
    cpu->d = 0;
    cpu->e = 0;
    cpu->h = 0;
    cpu->l = 0;

    cpu->cycles = 0;
}

void cpu_step(CPU *cpu, uint8_t *memory)
{
    uint8_t opcode = memory[cpu->pc];


    switch (opcode)
    {
        case 0x00:
            printf("NOP\n");
            cpu->pc++;
            cpu -> cycles +=4;
            break;

        case 0x21:
            printf("PUSH\n");

        case 0x31:
            printf("0x31\n");
            uint8_t low = memory[cpu->pc + 1];
            uint8_t high = memory[cpu->pc + 2];

            uint16_t immediate_value = ((uint16_t)high<<8) | low;

            cpu -> sp = immediate_value;
            cpu ->pc +=3;
            cpu -> cycles +=12;
            break;

        case 0xAF:
            printf("0xAF\n");
            cpu -> a ^= cpu -> a;
            cpu -> f ^= FLAG_Z;
            cpu -> cycles += 4;
            break;





        default:
            printf("Unknown opcode: 0x%02X at PC: 0x%04X\n", opcode, cpu->pc);
            cpu->pc++;
            cpu->cycles += 4;
            break;

    }

}