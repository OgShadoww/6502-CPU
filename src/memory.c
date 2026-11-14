#include <stdint.h>
#include <stdio.h>
#include "../emulator.h"

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
