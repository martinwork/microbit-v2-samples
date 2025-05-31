#include "MicroBit.h"

MicroBit uBit;

void forever()
{
    int level = 0;

    while (true)
    {
        level = uBit.audio.levelSPL->getValue();
        uBit.serial.printf( "%d:%d\r\n", (int) level, (int) system_timer_current_time_us());
        uBit.sleep(0);
    }
}

int main()
{
    uBit.init();
    create_fiber( forever);
    release_fiber();
}
