#include "MicroBit.h"

MicroBit uBit;

int main() {
    uBit.init();

    uBit.serial.printf("== start ==\r\n");
    uBit.serial.printf("P0.name=%d\r\n", uBit.io.P0.name);
    uBit.serial.printf("P1.name=%d\r\n", uBit.io.P1.name);
    uBit.serial.printf("P2.name=%d\r\n", uBit.io.P2.name);

    uBit.audio.setPin(uBit.io.P1);

    uBit.serial.printf("== end ==\r\n");
    uBit.serial.printf("P0.name=%d\r\n", uBit.io.P0.name);
    uBit.serial.printf("P1.name=%d\r\n", uBit.io.P1.name);
    uBit.serial.printf("P2.name=%d\r\n", uBit.io.P2.name);

    for(;;);
}
