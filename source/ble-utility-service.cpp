// ble-utility-service.cpp
// Test MicroBitUtilityService
// Press A to toggle logging - slow log of pitch and roll
// Press B to toggle logging - fast log to fill memory

#include "MicroBit.h"



#include "MicroBitUtilityService.h"

MicroBit uBit;

////////////////////////////////////////////////////////////////

int num_cols = 10; // The number of additional dummy columns (0 to 26)
int period   = 0;  // The logging loop period in ms (0 for minimum delay between iterations)

int loops = 0;
int logging1 = 0;
int logging2 = 0;


void datalogFull(MicroBitEvent e)
{
    if (logging1 != 0 || logging2 != 0) {
        logging1 = 0;
        logging2 = 0;
        uBit.display.printChar('F');
        uBit.audio.soundExpressions.play("giggle");
    }
}


void log2()
{
    const char *head = "a\0b\0c\0d\0e\0f\0g\0h\0i\0j\0k\0l\0m\0n\0o\0p\0q\0r\0s\0t\0u\0v\0w\0x\0y\0z\0";

    double delta = 0;
    double count = 0;
  
    period = 0;

    uBit.log.clear(false);
    uBit.log.setSerialMirroring(false);
    uBit.log.setTimeStamp(TimeStampFormat::Milliseconds);

    uBit.messageBus.listen( MICROBIT_ID_LOG, MICROBIT_LOG_EVT_LOG_FULL, datalogFull);

    while ( logging2)
    {
        CODAL_TIMESTAMP loop0 = system_timer_current_time();

        uBit.display.image.setPixelValue( 4, 0, uBit.display.image.getPixelValue( 4, 0) ? 0 : 255);

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

        CODAL_TIMESTAMP loop1 = system_timer_current_time();
        int loopTime = loop1 - loop0;
        int wait = period - loopTime;
        if ( wait < 0) wait = 0;
        uBit.sleep( wait);
    }

    release_fiber();
}



void log1()
{
    period = 500;

    uBit.log.clear(false);
    uBit.log.setSerialMirroring(false);
    uBit.log.setTimeStamp(TimeStampFormat::Milliseconds);

    uBit.messageBus.listen( MICROBIT_ID_LOG, MICROBIT_LOG_EVT_LOG_FULL, datalogFull);

    while ( logging1)
    {
        CODAL_TIMESTAMP loop0 = system_timer_current_time();

        uBit.display.image.setPixelValue( 0, 0, uBit.display.image.getPixelValue( 0, 0) ? 0 : 255);

        uBit.log.beginRow();
         
        uBit.log.logData( "pitch", ManagedString( (int) uBit.accelerometer.getPitch()));
        uBit.log.logData( "roll",  ManagedString( (int) uBit.accelerometer.getRoll()));

        uBit.log.endRow();

        CODAL_TIMESTAMP loop1 = system_timer_current_time();
        int loopTime = loop1 - loop0;
        int wait = period - loopTime;
        if ( wait < 0) wait = 0;
        uBit.sleep( wait);
    }

    release_fiber();
}

////////////////////////////////////////////////////////////////

void display()
{
    while (true)
    {
        while ( !logging1 && !logging2)
        {
            uBit.display.scroll( "A=LOG B=FILL", 100);
            uBit.sleep(500);
        }
        
        uBit.display.clear();
        
        while (logging1 || logging2)
        {
            uBit.sleep(500);
        }
    }
}


void onClickA( MicroBitEvent e)
{
    if ( logging1)
    {
      logging1 = 0;
    }
    else
    {
      logging2 = 0;
      uBit.sleep(1000);
      logging1 = 1;
      create_fiber( log1);
    }
}


void onClickB( MicroBitEvent e)
{
    if ( logging2)
    {
      logging2 = 0;
    }
    else
    {
      logging1 = 0;
      uBit.sleep(1000);
      logging2 = 1;
      create_fiber( log2);
    }
}


void onClickAB( MicroBitEvent e)
{
}


int main()
{
    uBit.init();

#if CONFIG_ENABLED(DEVICE_BLE) && CONFIG_ENABLED(MICROBIT_BLE_ENABLED)
    MicroBitUtilityService::createShared( *uBit.ble, uBit.messageBus, uBit.storage, uBit.log);
    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
    new MicroBitButtonService(*uBit.ble);
    new MicroBitLEDService(*uBit.ble, uBit.display);
#endif
    
    uBit.messageBus.listen(DEVICE_ID_BUTTON_A, DEVICE_BUTTON_EVT_CLICK, onClickA);
    uBit.messageBus.listen(DEVICE_ID_BUTTON_B, DEVICE_BUTTON_EVT_CLICK, onClickB);
    uBit.messageBus.listen(DEVICE_ID_BUTTON_AB, DEVICE_BUTTON_EVT_CLICK, onClickAB);

    create_fiber( display);
    release_fiber();
}
