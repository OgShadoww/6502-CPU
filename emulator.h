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
