// i2c-serial.cpp
// V2 only
//
// A simple, "no-fiber" datalogging test.
// Press button A to start or continue logging.
// Press button B to pause logging.
//
// When memory is full, the program clears memory and restarts logging.
// If the endRow function takes longer than 1000 ms,
// logging is stopped after 5000 ms and "HANG" is scrolled,
// so the serial output around the incident can be inspected.
// The current NRF52I2C.h occasionally really hangs, so "HANG" will not be seen.
//
// The variables num_cols and period can easily vary the logging regime.

#include "MicroBit.h"

MicroBit uBit;


int num_cols = 10; // The number of additional dummy columns (0 to 26)
int period   = 0;  // The logging loop period in ms (0 for minimum delay between iterations)

int loops = 0;
int logging = 0;


void datalogFull(MicroBitEvent e)
{
    if (logging != 0) {
        logging = 0;
        loops++;
        uBit.serial.send("FULL " + ManagedString( loops) + "\n");
        uBit.display.printChar('F');
        uBit.audio.soundExpressions.play("giggle");
        uBit.sleep( 1000);
        uBit.log.clear(true);
        uBit.display.scroll(loops);
        uBit.sleep( 1000);
        logging = 1;
    }
}


void onButtonA(MicroBitEvent e)
{
    logging = 1;
}


void onButtonB(MicroBitEvent e)
{
    logging = 0;
}

////////////////////////////////////////////////////////////////

int  main()
{
    uBit.init();

    uBit.serial.setBaud(CODAL_SERIAL_DEFAULT_BAUD_RATE);
    //uBit.serial.setBaud(9600);

    uBit.serial.setTxBufferSize(CODAL_SERIAL_DEFAULT_BUFFER_SIZE);
    //uBit.serial.setTxBufferSize(254);

    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,  MICROBIT_BUTTON_EVT_CLICK, onButtonB);

    const char *head = "a\0b\0c\0d\0e\0f\0g\0h\0i\0j\0k\0l\0m\0n\0o\0p\0q\0r\0s\0t\0u\0v\0w\0x\0y\0z\0";

    CODAL_TIMESTAMP debug_hangInEndRowTime = 0;

    double delta = 0;
    double count = 0;

    uBit.log.clear(true);
    uBit.log.setSerialMirroring(true);
    uBit.log.setTimeStamp(TimeStampFormat::Milliseconds);

    uBit.messageBus.listen( MICROBIT_ID_LOG, MICROBIT_LOG_EVT_LOG_FULL, datalogFull);

    while (true)
    {
        CODAL_TIMESTAMP loop0 = system_timer_current_time();

        if ( logging != 0)
        {
            uBit.display.image.setPixelValue( 0, 0, uBit.display.image.getPixelValue( 0, 0) ? 0 : 255);;

            uBit.log.beginRow();
             
            uBit.log.logData( "delta", ManagedString( (int) delta));
            uBit.log.logData( "txbuf", ManagedString( (int) uBit.serial.txBufferedSize()));

            for ( int col = 0; col < num_cols; col++)
            {
                uBit.log.logData( head[ col * 2], "1023");
            }

            double now0 = system_timer_current_time();
            uBit.log.endRow();
            double now1 = system_timer_current_time();

            count += 1;
            delta = now1 - now0;

            if ( now1 - now0 > 1000)
            {
                if ( debug_hangInEndRowTime == 0)
                    debug_hangInEndRowTime = now1;
            }

            if ( debug_hangInEndRowTime && ( now1 - debug_hangInEndRowTime) > 5000)
            {
                while (true)
                {
                    uBit.display.scroll("HANG");
                    uBit.sleep(100);
                }
            }
        }

        CODAL_TIMESTAMP loop1 = system_timer_current_time();
        int loopTime = loop1 - loop0;
        int wait = period - loopTime;
        if ( wait < 0) wait = 0;
        uBit.sleep( wait);
    }
}
