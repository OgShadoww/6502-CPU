#include <inttypes.h>
#include <stdio.h>
#include "../emulator.h"

void debug_current_step(const CPU *cpu) {
    BYTE p = cpu->flag_reg;

    printf("PC:%04X  SP:%02X  A:%02X  X:%02X  Y:%02X  "
           "P:%02X [%c%c%c%c%c%c%c%c]  CYC:%" PRIu64 "\n",
           (unsigned)cpu->pc, (unsigned)cpu->sp,
           (unsigned)cpu->a, (unsigned)cpu->x, (unsigned)cpu->y,
           (unsigned)p,
           p & 0x80 ? 'N' : '-',
           p & 0x40 ? 'V' : '-',
           '-', /* Bit 5 is not a normal mutable CPU flag. */
           p & 0x10 ? 'B' : '-',
           p & 0x08 ? 'D' : '-',
           p & 0x04 ? 'I' : '-',
           p & 0x02 ? 'Z' : '-',
           p & 0x01 ? 'C' : '-',
           cpu->cycles);
}
