//10s periodic timer event sets P0 high briefly
//P1 wakes
//P2 is collecting analogue

//To choose sleep mode, Reset then
//A for deepSleep
//B for deepSleep(milliseconds)
//AB for deepSleep(P1)
//P1 for no deep sleep

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
uint16_t timer_value = 0;
CODAL_TIMESTAMP timer_period = 10000; //ms

int accel_x;
int p2_analogue;
int light_level;

int sleepMode = 0;


void doSleep()
{
    switch ( sleepMode)
    {
        case 0: uBit.power.deepSleep(); break;
        case 1: uBit.power.deepSleep( timer_period - 1000); break;
        case 2: uBit.power.deepSleep( uBit.io.P1); break;
        case 3: uBit.sleep( timer_period + 1000); break;
    }
}


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
    uBit.sleep(1000);

    DMESGN( "%u:onTimer before doSleep ", (unsigned int) system_timer_current_time_us());
    sendTime();

    doSleep();

    DMESGN( "%u:onTimer after  doSleep ", (unsigned int) system_timer_current_time_us());
    sendTime();
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

    DMESGN( "%u:onEventAndSleep before doSleep ", (unsigned int) system_timer_current_time_us());
    sendTime();

    doSleep();

    DMESGN( "%u:onEventAndSleep after doSleep ", (unsigned int) system_timer_current_time_us());
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

    //DMESGN( "onEvent EXIT ");
    //sendTime();
}


void onClickA(MicroBitEvent e)
{
    DMESGN( "%u:onClickA %d, %d, %u ", (unsigned int) system_timer_current_time_us(), (int) e.source, (int) e.value, (unsigned int) e.timestamp);
    sendTime();

    uBit.display.printCharAsync('A');

    accel_x = uBit.accelerometer.getX();
    p2_analogue = uBit.io.P2.getAnalogValue();
    light_level = uBit.display.readLightLevel();
    DMESG( "onEvent p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);


    DMESGN( "%u:onClickA before doSleep ", (unsigned int) system_timer_current_time_us());
    sendTime();

    doSleep();

    DMESGN( "%u:onClickA after doSleep ", (unsigned int) system_timer_current_time_us());
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
    DMESG( "onEvent p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);

    //DMESGN( "onEvent EXIT ");
    //sendTime();
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

    //uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_EVT_ANY, onEvent);
    //uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,  MICROBIT_EVT_ANY, onEvent);
    uBit.messageBus.listen( MICROBIT_ID_IO_P1,     MICROBIT_EVT_ANY, onEventAndSleep);
    uBit.messageBus.listen( MICROBIT_ID_IO_P2,     MICROBIT_EVT_ANY, onEvent);
    //uBit.messageBus.listen( MICROBIT_ID_IO_P5,     MICROBIT_EVT_ANY, onEvent);
    //uBit.messageBus.listen( MICROBIT_ID_IO_P11,    MICROBIT_EVT_ANY, onEventAndSleep);
    uBit.messageBus.listen( timer_id, timer_value, onTimer);

    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onClickA);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,  MICROBIT_BUTTON_EVT_CLICK, onClickB);

    uBit.io.P1.wakeOnActive(1);
    uBit.io.P2.wakeOnActive(1);
    uBit.io.buttonA.wakeOnActive(1);
    uBit.io.buttonB.wakeOnActive(1);

    DMESG( "P1  wake %d", uBit.io.P1.isWakeOnActive());
    DMESG( "P2  wake %d", uBit.io.P2.isWakeOnActive());
    DMESG( "P5  wake %d", uBit.io.P5.isWakeOnActive());
    DMESG( "P11 wake %d", uBit.io.P11.isWakeOnActive());

    system_timer_event_every( timer_period, timer_id, timer_value, CODAL_TIMER_EVENT_FLAGS_WAKEUP);

    while (true)
    {
        accel_x = uBit.accelerometer.getX();
        p2_analogue = uBit.io.P2.getAnalogValue();
        light_level = uBit.display.readLightLevel();
        DMESG( "test_forever p2_analogue %d light_level %d accel_x %d", p2_analogue, light_level, accel_x);
        uBit.sleep(2000);
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