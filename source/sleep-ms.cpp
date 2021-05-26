//10s periodic timer event sets P0 high briefly
//P1 wakes
//P2 is collecting analogue

//To choose sleep mode, Reset then
//A for deepSleep
//B for deepSleep(milliseconds)
//AB for deepSleep(P1)
//P1 for no deep sleep

/*
Thinking about using deepSleep in a forever loop...

I'm struggling to think of an application, unless I really wanted to avoid using event handlers or there weren't any that would call deepSleep. deepSleepAsync doesn't seem to make any sense.

system_timer_event_after(period) + uBit.power.deepSleep() has the effect of deep sleep for up to period. Compare uBit.sleep(period), which does sleep for at least period.

*/

#include "MicroBit.h"

#ifndef MICROBIT_CODAL
#ifdef CODAL_CONFIG_H
#define MICROBIT_CODAL 1
#else
#define MICROBIT_CODAL 0
#endif
#endif

MicroBit uBit;

void togglePixel( int x, int y);

uint64_t time0 = 0;
uint64_t time1 = 0;
uint64_t timeD = 0;

uint64_t second = 1000000;

uint16_t timer_id = 60000;
uint16_t timer_value = 1;
CODAL_TIMESTAMP timer_period = 20000; //ms

CODAL_TIMESTAMP forever_sleep = 3000; //ms

int accel_x;
int p2_analogue;
int light_level;

int sleepMode = 0;



void sendTime()
{
    time1 = system_timer_current_time_us();
    timeD = time1 - time0;

    int us  = (int) (timeD % second);
    int sec = (int) (timeD / second);
    int min = sec / 60;
    sec = sec % 60;

    ManagedString sm( min);
    ManagedString ss( sec);  ss = "00"     + ss; ss = ss.substring( ss.length() - 2, 2);
    ManagedString su( us);   su = "000000" + su; su = su.substring( su.length() - 6, 6);
    ManagedString msg = "(time " + sm + ":" + ss + "." + su + ")";
    DMESG( msg.toCharArray());
}

////////////////////////////////////////////////////////////////

void onTimer(MicroBitEvent e)
{
    DMESGN( "%u:ONTIMER ", (unsigned int) system_timer_current_time_us());
    sendTime();

    uBit.io.P0.setDigitalValue(1);
    togglePixel( 3, 0);
    uBit.sleep(2000);
    togglePixel( 3, 0);
    uBit.io.P0.setDigitalValue(0);

    accel_x = uBit.accelerometer.getX();
    p2_analogue = uBit.io.P2.getAnalogValue();
    light_level = uBit.display.readLightLevel();
    DMESG( "onTimer p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);

    uBit.power.deepSleep();
}


void onEventAndSleep(MicroBitEvent e)
{
    DMESGN( "%u:ONEVENTAndSleep %d, %d, %u ", (unsigned int) system_timer_current_time_us(), (int) e.source, (int) e.value, (unsigned int) e.timestamp);
    sendTime();

    accel_x = uBit.accelerometer.getX();
    p2_analogue = uBit.io.P2.getAnalogValue();
    light_level = uBit.display.readLightLevel();
    DMESG( "onEvent p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);

    //DMESGN( "onEventAndSleep EXIT ");
    //sendTime();

    DMESGN( "%u:onEventAndSleep before deepSleepAsync ", (unsigned int) system_timer_current_time_us());
    sendTime();

    uBit.power.deepSleepAsync();

    DMESGN( "%u:onEventAndSleep after deepSleepAsync ", (unsigned int) system_timer_current_time_us());
    sendTime();
}


void onEvent(MicroBitEvent e)
{
    DMESGN( "%u:ONEVENT %d, %d, %u ", (unsigned int) system_timer_current_time_us(), (int) e.source, (int) e.value, (unsigned int) e.timestamp);
    sendTime();

    accel_x = uBit.accelerometer.getX();
    p2_analogue = uBit.io.P2.getAnalogValue();
    light_level = uBit.display.readLightLevel();
    DMESG( "onEvent p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);
}


void onClickA(MicroBitEvent e)
{
    DMESGN( "%u:onClickA %d, %d, %u ", (unsigned int) system_timer_current_time_us(), (int) e.source, (int) e.value, (unsigned int) e.timestamp);
    sendTime();

    uBit.display.printCharAsync('A');

    accel_x = uBit.accelerometer.getX();
    p2_analogue = uBit.io.P2.getAnalogValue();
    light_level = uBit.display.readLightLevel();
    DMESG( "onClickA p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);


    DMESGN( "%u:onClickA before deepSleep ", (unsigned int) system_timer_current_time_us());
    sendTime();

    uBit.power.deepSleep();

    DMESGN( "%u:onClickA after deepSleep ", (unsigned int) system_timer_current_time_us());
    sendTime();
}


void onClickB(MicroBitEvent e)
{
    DMESGN( "%u:onClickB %d, %d, %u ", (unsigned int) system_timer_current_time_us(), (int) e.source, (int) e.value, (unsigned int) e.timestamp);
    sendTime();

    uBit.display.printCharAsync('B');

    accel_x = uBit.accelerometer.getX();
    p2_analogue = uBit.io.P2.getAnalogValue();
    light_level = uBit.display.readLightLevel();
    DMESG( "onClickB p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);
}


void test_forever()
{
    DMESGN( "test_forever ");
    sendTime();
    time0 = time1;

    uBit.radio.datagram.send( ManagedString("onstart"));
    uBit.io.P0.setDigitalValue(0);

    accel_x = uBit.accelerometer.getX();
    p2_analogue = uBit.io.P2.getAnalogValue();
    light_level = uBit.display.readLightLevel();
    DMESG( "test_forever p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);

    uBit.io.P1.setPull(PullMode::Down);
    uBit.io.P1.eventOn(DEVICE_PIN_EVENT_ON_EDGE);

    uBit.messageBus.listen( MICROBIT_ID_IO_P1,     MICROBIT_EVT_ANY, onEventAndSleep);
    uBit.messageBus.listen( MICROBIT_ID_IO_P2,     MICROBIT_EVT_ANY, onEvent);

    uBit.messageBus.listen( timer_id, timer_value, onTimer);

    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onClickA);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,  MICROBIT_BUTTON_EVT_CLICK, onClickB);

    uBit.io.P1.wakeOnActive(1);
    uBit.io.P2.wakeOnActive(1);
    uBit.io.buttonA.wakeOnActive(1);
    uBit.io.buttonB.wakeOnActive(1);

    DMESG( "P1  wake %d", uBit.io.P1.getWakeOnActive());
    DMESG( "P2  wake %d", uBit.io.P2.getWakeOnActive());
    DMESG( "P5  wake %d", uBit.io.P5.getWakeOnActive());
    DMESG( "P11 wake %d", uBit.io.P11.getWakeOnActive());

    system_timer_event_every( timer_period, timer_id, timer_value, CODAL_TIMER_EVENT_FLAGS_WAKEUP);

    uBit.sleep(100);

    while (true)
    {
        accel_x = uBit.accelerometer.getX();
        p2_analogue = uBit.io.P2.getAnalogValue();
        light_level = uBit.display.readLightLevel();
        DMESGN( "%u:test_forever p2_analogue %d light_level %d accel_x %d ", (unsigned int) system_timer_current_time_us(), p2_analogue, light_level, accel_x);
        sendTime();
        togglePixel( 4, 4);
        uBit.sleep(1000);
        togglePixel( 4, 4);
        DMESGN( "%u:test_forever ", (unsigned int) system_timer_current_time_us());
        sendTime();
        //uBit.power.deepSleepYield();
        uBit.power.deepSleep(forever_sleep);
    }
}

////////////////////////////////////////////////////////////////

void onButtonA(MicroBitEvent e);
void onButtonB(MicroBitEvent e);
void onButtonAB(MicroBitEvent e);
void onPin(MicroBitEvent e);

void start_test( int mode)
{
    DMESG( "start_test %d", mode);
    uBit.messageBus.ignore( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.ignore( MICROBIT_ID_BUTTON_B,  MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.messageBus.ignore( MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonAB);
    uBit.messageBus.ignore( MICROBIT_ID_IO_P1,     MICROBIT_EVT_ANY,          onPin);
    sleepMode = mode;
    create_fiber( test_forever);
}

void onButtonA(MicroBitEvent e)
{
    DMESG( "onButtonA");
    start_test( 0);
}


void onButtonB(MicroBitEvent e)
{
    DMESG( "onButtonB");
    start_test( 1);
}


void onButtonAB(MicroBitEvent e)
{
    DMESG( "onButtonAB");
    start_test( 2);
}

void onPin(MicroBitEvent e)
{
    DMESG( "onPin %d, %d ", (int) e.source, (int) e.value);
    start_test( 3);
}

////////////////////////////////////////////////////////////////

void togglePixel( int x, int y)
{
    uBit.display.image.setPixelValue( x, y, uBit.display.image.getPixelValue( x, y) ? 0 : 255);
}


void forever()
{
    uBit.power.deepSleepYieldAsync(true);

    while (true)
    {
        //DMESG( "forever ");
        togglePixel( 0, 0);
        uBit.sleep(1000);
    }
}


int  main()
{
    uBit.init();

    accel_x = uBit.accelerometer.getX();
    p2_analogue = uBit.io.P2.getAnalogValue();
    light_level = uBit.display.readLightLevel();
    DMESG( "onEvent p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);

    create_fiber( forever);

    uBit.io.P1.setPull(PullMode::Down);
    uBit.io.P1.eventOn(DEVICE_PIN_EVENT_ON_EDGE);

    uBit.messageBus.listen( MICROBIT_ID_IO_P1,     MICROBIT_EVT_ANY,          onPin);

    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,  MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonAB);

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