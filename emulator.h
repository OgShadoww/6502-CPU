#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

#define RAM_MEM (1024 * 48)
#define ROM_MEM (1024 * 16)

typedef uint8_t BYTE;
typedef uint16_t WORD;

typedef struct {
    BYTE data[RAM_MEM];
} RAM;

typedef struct {
    BYTE data[ROM_MEM];
} ROM;

typedef struct {
    RAM *ram;
    ROM *rom;
} BUS;

typedef struct {
    WORD pc;
    BYTE sp;

    BYTE a, x, y;
    BYTE flag_reg;

    uint64_t cycles;
    BUS *bus;
} CPU;

typedef enum {
    FLAG_C = 0x01,
    FLAG_Z = 0x02,
    FLAG_I = 0x04,
    FLAG_D = 0x08,
    FLAG_B = 0x10,
    FLAG_U = 0x20,
    FLAG_V = 0x40,
    FLAG_N = 0x80
} FLAGS;

typedef enum {
    INS_LDA_IM = 0xA9,
    INS_LDX_IM = 0xA2,
    INS_LDY_IM = 0xA0
} INS;

// Memory
int rom_load(ROM *rom, char *file_name);

// Bus
BYTE bus_read(WORD address, const BUS *bus);
void bus_write(WORD address, BYTE value, BUS *bus);

// CPU
void reboot(CPU *cpu, BUS *bus);
int cpu_step(CPU *cpu);

// Debugger
void debug_current_step(const CPU *cpu);

#endif
