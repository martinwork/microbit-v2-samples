
#include "MicroBit.h"


MicroBit uBit;

void togglePixel( int x, int y);

uint16_t timer_id = 60000;
uint16_t timer_value = 1;
CODAL_TIMESTAMP timer_period = 25000; //ms

CODAL_TIMESTAMP forever_sleep = 3000; //ms


void sendTime( const char *suffix)
{
    uint64_t second = 1000000;

    uint64_t now = system_timer_current_time_us();

    int u  = (int) (now % second);
    int s = (int) (now / second);
    int m = s / 60;
    s = s % 60;
    int h  = m / 60;
    m = m % 60;

    ManagedString sh( h);
    ManagedString sm( m);  sm = "00"     + sm; sm = sm.substring( sm.length() - 2, 2);
    ManagedString ss( s);  ss = "00"     + ss; ss = ss.substring( ss.length() - 2, 2);
    ManagedString su( u);  su = "000000" + su; su = su.substring( su.length() - 6, 6);
    ManagedString msg = sh + ":" + sm + ":" + ss + "." + su + " " + ManagedString(suffix);
    uBit.serial.send( msg);
}

////////////////////////////////////////////////////////////////

void onTimer(MicroBitEvent e)
{
#if CONFIG_ENABLED(DEVICE_DEEPSLEEP_DEFAULT_ALLOW)
    uBit.power.deepSleepYieldAsync(0);
#endif

    sendTime( "onTimer\n");

    int light = 0;

    for ( int i = 0; i < 5; i++)
    {
      light += uBit.display.readLightLevel();
      uBit.sleep(1000);
    }
    light /= 10;

    sendTime("");
    uBit.serial.send( "light = " + ManagedString( light) + "\n");

    uBit.power.deepSleep();
}


void onClickA(MicroBitEvent e)
{
#if CONFIG_ENABLED(DEVICE_DEEPSLEEP_DEFAULT_ALLOW)
    uBit.power.deepSleepYieldAsync(0);
#endif

    sendTime( "onClickA\n");

    uBit.display.scroll("ON CLICK A");

    sendTime( "onClickA before\n");

    uBit.power.deepSleep();

    sendTime( "onClickA after\n");

    uBit.display.scroll("AFTER A");
}


void onClickB(MicroBitEvent e)
{
#if CONFIG_ENABLED(DEVICE_DEEPSLEEP_DEFAULT_ALLOW)
    uBit.power.deepSleepYieldAsync(0);
#endif

    sendTime( "onClickB\n");

    uBit.display.scroll("ON CLICK B");

    for ( int i = 0; i < 10; i++)
    {
      int f = 200 + 40 * i;
      uint32_t us = floor( 0.5 + 1000000.0 / f);
      uBit.audio.virtualOutputPin.setAnalogPeriodUs( us);
      uBit.audio.virtualOutputPin.setAnalogValue(128);
      uBit.sleep(200);
    }
    uBit.audio.virtualOutputPin.setAnalogValue(0);
    uBit.audio.virtualOutputPin.setAnalogPeriodUs( 20000);

    sendTime( "onClickB before\n");

    uBit.power.deepSleep();

    sendTime( "onClickB after\n");

    uBit.display.scroll("AFTER B");
}


void milliseconds()
{
#if CONFIG_ENABLED(DEVICE_DEEPSLEEP_DEFAULT_ALLOW)
    uBit.power.deepSleepYieldAsync(0);
#endif

    while (true)
    {
        togglePixel( 4, 0);
        uBit.sleep(1000);
        togglePixel( 4, 0);

        sendTime( "milliseconds before\n");

        uBit.power.deepSleep(forever_sleep);

        sendTime( "milliseconds after\n");
        uBit.sleep(20);
    }
}


void test()
{
    sendTime( "test\n");

    uBit.messageBus.listen( timer_id, timer_value, onTimer);

    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onClickA);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,  MICROBIT_BUTTON_EVT_CLICK, onClickB);

    uBit.io.buttonA.wakeOnActive(1);
    uBit.io.buttonB.wakeOnActive(1);

    system_timer_event_every( timer_period, timer_id, timer_value, CODAL_TIMER_EVENT_FLAGS_WAKEUP);

    create_fiber( milliseconds);

    while (true)
    {
        togglePixel( 4, 4);
        uBit.sleep(100);
        togglePixel( 4, 4);

#if CONFIG_ENABLED(DEVICE_DEEPSLEEP_DEFAULT_ALLOW)
#else
        uBit.power.deepSleepYield();
#endif
        uBit.sleep(20);
    }
}

////////////////////////////////////////////////////////////////

void togglePixel( int x, int y)
{
    uBit.display.image.setPixelValue( x, y, uBit.display.image.getPixelValue( x, y) ? 0 : 255);
}


void forever()
{
    while (true)
    {
        togglePixel( 0, 0);
        uBit.sleep(1000);
    }
}


int  main()
{
    uBit.init();

    create_fiber( forever);
    create_fiber( test);

#if CONFIG_ENABLED(MICROBIT_BLE_ENABLED)
    //uBit.accelerometer.setPeriod(80);

    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitButtonService(*uBit.ble);
    new MicroBitIOPinService(*uBit.ble, uBit.io);
    new MicroBitLEDService(*uBit.ble, uBit.display);
    //new MicroBitMagnetometerService(*uBit.ble, uBit.compass);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
    //new MicroBitUARTService(*uBit.ble);

    //uBit.compass.heading();
#endif

    release_fiber();
}