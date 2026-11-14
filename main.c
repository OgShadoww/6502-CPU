#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>

#define RAM_MEM 1024 * 48
#define ROM_MEM 1024 * 16

typedef uint8_t BYTE;
typedef uint16_t WORD;

enum {
    INS_LDA_IM = 0xA9,
    INS_LDX_IM = 0xA2,
    INS_LDY_IM = 0xA0
};

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

void debug_current_step(const CPU *cpu) {
    BYTE p = cpu->flag_reg;

    printf(
        "PC:%04X  SP:%02X  "
        "A:%02X  X:%02X  Y:%02X  "
        "P:%02X [%c%c%c%c%c%c%c%c]  "
        "CYC:%" PRIu64 "\n",
        (unsigned)cpu->pc,
        (unsigned)cpu->sp,
        (unsigned)cpu->a,
        (unsigned)cpu->x,
        (unsigned)cpu->y,
        (unsigned)p,
        p & 0x80 ? 'N' : '-',
        p & 0x40 ? 'V' : '-',
        '-',
        p & 0x10 ? 'B' : '-',
        p & 0x08 ? 'D' : '-',
        p & 0x04 ? 'I' : '-',
        p & 0x02 ? 'Z' : '-',
        p & 0x01 ? 'C' : '-',
        cpu->cycles
    );
}

BYTE bus_read(WORD adress, BUS *bus) {
  if(adress < 0xC000) {
    return bus->ram->data[adress];
  }
  else {
    return bus->rom->data[adress - 0xC000];
  }

  return 0;
}

void bus_write(WORD adress, BYTE data, BUS *bus) {
  if(adress < 0xC000) {
    bus->ram->data[adress] = data;
  }
  else {
    return;
  }
}

int rom_load(ROM *rom, char *file_name) {
  FILE *file = fopen(file_name, "rb");
  if(!file) {
    fclose(file);
    return -1;
  }

  size_t size = fread(rom, 1, sizeof(rom->data), file);
  fclose(file);
  
  if(size != sizeof(rom->data)) {
    printf("Error reading ROM image file : %s", file_name);
    return -1;
  }

  return 0;
}

void reboot(CPU *cpu, BUS *bus) {
  // Reset CPU values
  cpu->a = 0; cpu->x = 0; cpu->y = 0;
  cpu->flag_reg = 0;
  cpu->cycles = 0;
  cpu->bus = bus;

  BYTE low_byte = bus_read(0xFFFC, cpu->bus);
  BYTE high_byte = bus_read(0xFFFD, cpu->bus);
  cpu->pc = low_byte | (high_byte << 8);
  cpu->sp = 0xFD;
}

BYTE fetch(CPU *cpu) {
  BYTE data = bus_read(cpu->pc, cpu->bus);
  cpu->pc++;

  return data;
}

WORD fetch_word(CPU *cpu) {
  BYTE lower_bytes = bus_read(cpu->pc, cpu->bus);
  cpu->pc++;
  BYTE upper_bytes = bus_read(cpu->pc, cpu->bus);
  cpu->pc++;
  WORD data = lower_bytes | (upper_bytes << 8);

  return data;
}

void cpu_step(CPU *cpu) {
  BYTE instruction = fetch(cpu);

  switch (instruction) {
    case INS_LDA_IM: {
      BYTE data = fetch(cpu);
      cpu->a = data;
      cpu->cycles += 2;
      break;
    }
    case INS_LDX_IM: {
      BYTE data = fetch(cpu);
      cpu->x = data;
      cpu->cycles += 2;
      break;
    }
    case INS_LDY_IM: {
      BYTE data = fetch(cpu);
      cpu->y = data;
      cpu->cycles += 2;
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
  // Init
  CPU cpu;
  RAM ram;
  ROM rom = {0};
  BUS bus = {.rom = &rom, .ram = &ram};
  reboot(&cpu, &bus);

  bus_write(cpu.pc, INS_LDA_IM, &bus);
  bus_write(cpu.pc + 1, 0xA, &bus);
  bus_write(cpu.pc + 2, INS_LDX_IM, &bus);
  bus_write(cpu.pc + 3, 0xB, &bus);
  bus_write(cpu.pc + 4, INS_LDY_IM, &bus);
  bus_write(cpu.pc + 5, 0xC, &bus);
  
  // Running
  char c;
  while(1) {
    while((c = getchar()) != '\n');
    debug_current_step(&cpu);
    cpu_step(&cpu);
  }
}
