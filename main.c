#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define RAM_MEM 1024 * 48
#define ROM_MEM 1024 * 16

typedef uint8_t BYTE;
typedef uint16_t WORD;

const BYTE INS_LDA_IM = 0xA9;
const BYTE INS_LDX_IM = 0xA2;
const BYTE INS_LDY_IM = 0xA0;

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

  BYTE a,x,y;
  BYTE flag_reg; // c, z, i, d, b, o, n

  uint64_t cycles;
  BUS *bus;
} CPU;


BYTE bus_read(WORD adress, BUS *bus) {
  if(adress < 0xC000) {
    return bus->ram->data[adress];
  }
  else if(adress >= 0xC000 && adress <= 0xFFFF) {
    return bus->rom->data[adress - 0xC000];
  }

  return 0;
}

void bus_write(WORD adress, BYTE data, BUS *bus) {
  if(adress < 0xC000) {
    bus->ram->data[adress] = data;
  }
  else if(adress >= 0xC000 && adress < 0xFFFF) {
    return;
  }
}

void reboot(CPU *cpu, RAM *ram, ROM *rom, BUS *bus) {
  cpu->pc = 0xFFFC;
  cpu->sp = 0xff;

  cpu->a = 0; cpu->x = 0; cpu->y = 0;
  cpu->flag_reg = 0;
  cpu->cycles = 0;

  bus->ram = ram;
  bus->rom = rom;
  cpu->bus = bus;
}

BYTE fetch(CPU *cpu) {
  BYTE data = bus_read(cpu->pc, cpu->bus);
  cpu->pc++;

  return data;
}

void cpu_step(CPU *cpu) {
  BYTE instruction = fetch(cpu);

  switch (instruction) {
    case INS_LDA_IM: {
      BYTE data = fetch(cpu);
      cpu->a = data;
      break;
    }
    case INS_LDX_IM: {
      BYTE data = fetch(cpu);
      cpu->x = data;
      break;
    }
    case INS_LDY_IM: {
      BYTE data = fetch(cpu);
      cpu->y = data;
      break;
    }

    default: {
      printf("The emulator has stopped becuase of unknown opcode: %d; at adress: %d", instruction, cpu->pc-1);
      exit(1);
      return;
    }
  }
}

int main() {
  CPU cpu;
  RAM ram;
  ROM rom;
  BUS bus;

  reboot(&cpu, &ram, &rom, &bus);

  while(1) {
    cpu_step(&cpu);
  }
}
