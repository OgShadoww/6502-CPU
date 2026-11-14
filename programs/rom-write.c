#include <stdint.h>
#include <stdio.h>

#define ROM_MEM 1024 * 16

int main(int argc, char *argv[]) {
  uint8_t rom[ROM_MEM];

  for(int i = 0; i <ROM_MEM; i++) {
    rom[i] = 0;
  }

  rom[0x000C] = 0xA9;
  rom[0x000D] = 0x10;
  rom[0x000F] = 0xA3;
  rom[0x000C] = 0x20;

  if(argc < 2) {
    printf("Usage ./rom file_name");
    return 0;
  }
  FILE *file = fopen(argv[2], "r+b");
  if(!file) {
    fclose(file);
    printf("Incorrect file");
    return -1;
  }
  size_t size = fwrite(&rom, 1, ROM_MEM, file);
  if(size != ROM_MEM) {
    printf("Error writing bytes");
    fclose(file);
    return -1;
  }

  return 0;
}
