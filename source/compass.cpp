#include "MicroBit.h"

MicroBit uBit;


void onButtonA(MicroBitEvent)
{
    //uBit.compass.clearCalibration();
    uBit.compass.calibrate();
    CompassCalibration cal = uBit.compass.getCalibration();
    uBit.serial.printf( "cal centre %d %d %d scale %d %d %d radius %d\n", cal.centre.x, cal.centre.y, cal.centre.z, cal.scale.x,  cal.scale.y,  cal.scale.z, cal.radius);
}



void forever()
{
    int heading = uBit.compass.heading();
    CompassCalibration cal = uBit.compass.getCalibration();
    uBit.serial.printf( "cal centre %d %d %d scale %d %d %d radius %d\n", cal.centre.x, cal.centre.y, cal.centre.z, cal.scale.x,  cal.scale.y,  cal.scale.z, cal.radius);

    while(1)
    {
        heading = uBit.compass.heading();
        if ( heading < 45)
            uBit.display.print("N");
        else if ( heading < 135)
            uBit.display.print("E");
        else if ( heading < 225)
            uBit.display.print("S");
        else if ( heading < 315)
            uBit.display.print("W");
        else
            uBit.display.print("N");

        uBit.sleep(20);
    }
}


int 
main()
{
    uBit.init();

    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onButtonA);

    create_fiber(forever);

    release_fiber();
}