#include "cpu.h"
#include <stdio.h>
#include "mmu.h"


uint16_t get_HL(CPU *cpu)
{
    return ((uint16_t)cpu -> h << 8) | cpu -> l;
}

void set_HL(CPU *cpu, uint16_t value)
{
    cpu->h = (uint8_t)(value >> 8);
    cpu->l = (uint8_t)(value & 0xFF);
}

void cpu_execute_cb(CPU *cpu, uint8_t *memory)
{
    uint8_t cb_opcode = mmu_read_byte(memory, cpu -> pc);

    printf("CB opcode: 0x%02X\n",cb_opcode);
    switch (cb_opcode)
    {
        case 0x00:
            {
                uint8_t val = cpu -> b;
                uint8_t carry_bit = (val >> 7) & 1;

                cpu -> b = (val << 1) | carry_bit;

                cpu -> f = 0;

                if (cpu -> b == 0)
                {
                    cpu -> f |= FLAG_Z;
                }

                if (carry_bit)
                {
                    cpu -> f |= FLAG_C;
                }

                cpu -> cycles += 8;

                break;
            }

        case 0x7C:
            {

                if (!((cpu -> h >>7) & 1))
                {
                    cpu -> f |= FLAG_Z;
                } else
                {

                    cpu -> f &= ~FLAG_Z;
                }

                cpu -> f &= ~FLAG_N;
                cpu -> f |= FLAG_H;

                cpu -> cycles +=8;
                break;
            }

    default:
            {
                printf("Unknown CB-prefixed opcode: 0x%02X at PC: 0x%04X\n", cb_opcode, cpu->pc);

                cpu -> cycles += 8;
                break;
            }
    }
}

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

// Function: cpu_step
void cpu_step(CPU *cpu, uint8_t *memory)
{
    uint8_t opcode = mmu_read_byte(memory, cpu->pc);

    switch (opcode)
    {
        case 0x00:
            {
                printf("NOP\n");
                cpu->pc++;
                cpu -> cycles +=4;
                break;
            }

        case 0x21: // Assuming this is LD HL, d16
            {
                printf("LD HL, d16 (0x21)\n");
                uint8_t low = mmu_read_byte(memory, cpu->pc + 1);
                uint8_t high = mmu_read_byte(memory, cpu->pc + 2);
                cpu->h = high;
                cpu->l = low;
                cpu->pc += 3;
                cpu->cycles += 12;
                break;
            }
        case 0x31:
        {
            printf("LD SP, d16 (0x31)\n");
            uint8_t low = mmu_read_byte(memory, cpu->pc + 1);
            uint8_t high = mmu_read_byte(memory, cpu->pc + 2);

            uint16_t immediate_value = ((uint16_t)high<<8) | low;

            cpu -> sp = immediate_value;
            cpu ->pc +=3;
            cpu -> cycles +=12;
            break;
        }

        case 0x32: // LD (HL-), A
        {
            printf("LD (HL-), A (0x32)\n");
            uint16_t address = get_HL(cpu);
            mmu_write_byte(memory, address, cpu -> a);


            set_HL(cpu,address-1);

            cpu->pc++;
            cpu->cycles += 8;
            break;
        }

        case 0xAF:
        {
            printf("XOR A (0xAF)\n");
            cpu -> a ^= cpu -> a;
            cpu->f = FLAG_Z;
            cpu -> pc++;
            cpu -> cycles += 4;
            break;
        }

    case 0xCB:
            {
                cpu -> pc++;
                cpu_execute_cb(cpu, memory);

                break;
            }

        default:
        {
            printf("Unknown opcode: 0x%02X at PC: 0x%04X\n", opcode, cpu->pc);
            cpu->pc++;
            cpu->cycles += 4;
            break;
        }
    }

}