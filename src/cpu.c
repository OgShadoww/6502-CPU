#include <stdio.h>
#include "../emulator.h"

enum {
    FLAG_C = 0x01,
    FLAG_Z = 0x02,
    FLAG_I = 0x04,
    FLAG_D = 0x08,
    FLAG_B = 0x10,
    FLAG_U = 0x20,
    FLAG_V = 0x40,
    FLAG_N = 0x80
};

enum {
    INS_LDA_IM = 0xA9,
    INS_LDX_IM = 0xA2,
    INS_LDY_IM = 0xA0
};


void cpu_reset(CPU *cpu, BUS *bus) {
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

int cpu_step(CPU *cpu) {
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
      return -1;
    }
  }
}
