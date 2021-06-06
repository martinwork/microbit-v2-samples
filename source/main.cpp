#include "MicroBit.h"

MicroBit uBit;

int 
main()
{
    uBit.init();

    MicroBitVersion v = uBit.power.getVersion();
    ManagedString i( (int) v.i2c);
    ManagedString d( (int) v.daplink);
    ManagedString b( (int) v.board);
    ManagedString msg = "i " + i + " d " + d + " b " + b + "\n";
    uBit.serial.send( msg);

    while(1)
    {
    }
}
