#include "../emulator.h"

#define ROM_BASE 0xC000

BYTE bus_read(WORD address, const BUS *bus) {
    if (address < ROM_BASE) {
        return bus->ram->data[address];
    }
    return bus->rom->data[address - ROM_BASE];
}

void bus_write(WORD address, BYTE value, BUS *bus) {
    if (address < ROM_BASE) {
        bus->ram->data[address] = value;
    }
}
