#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>
#include "../emulator.h"

int main() {
  // Init
  CPU cpu;
  RAM ram;
  ROM rom = {0};
  BUS bus = {.rom = &rom, .ram = &ram};
  rom_load(&rom, "rom.bin");
  reboot(&cpu, &bus);

  // Running
  char c;
  while(1) {
    while((c = getchar()) != '\n');
    debug_current_step(&cpu);
    cpu_step(&cpu);
  }
}
