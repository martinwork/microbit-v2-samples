// NEOPIXEL TESTS

#include "MicroBit.h"
#include "limits.h"

MicroBit uBit;

#define BUFFERMAX (256 * 3)

int bufferSize = 30;
int stopTime = 0;  // 520, 
int sleep = 0;

uint8_t *buffer = NULL;

ManagedString output = "";

bool buttonA = false;
bool buttonB = false;


void dmesg_flush()
{
#if DEVICE_DMESG_BUFFER_SIZE > 0
    if (codalLogStore.ptr > 0 && microbit_device_instance)
    {
        for (uint32_t i=0; i<codalLogStore.ptr; i++)
            uBit.serial.putc(codalLogStore.buffer[i]);

        codalLogStore.ptr = 0;
    }
#endif
}

void sendOutput()
{
    ManagedString send = "\r\n" + ManagedString( (int) (system_timer_current_time_us() % INT_MAX)) + "\r\n" + output;
    output = "";
    uBit.serial.send( send);
}

void onButtonA(MicroBitEvent e)
{
    buttonA = true;
}

void onButtonB(MicroBitEvent e)
{
    buttonB = true;
}

void forever()
{
#if DEVICE_DMESG_BUFFER_SIZE > 0
    codal_dmesg_set_flush_fn( dmesg_flush);
#endif

    uBit.sleep(1000);

    uBit.serial.printf(
      "bufferSize %d sleep %d DEVICE_DMESG_BUFFER_SIZE %d\r\n",
      bufferSize, sleep, DEVICE_DMESG_BUFFER_SIZE);

    uBit.sleep(500);

    buffer = (uint8_t *) malloc( BUFFERMAX);
    for ( int idx = 0; idx < BUFFERMAX; idx++)
    {
      buffer[ idx] = 1 + ( idx % 9);
    }
    neopixel_send_buffer( uBit.io.pin[0], buffer, bufferSize);

    uBit.sleep(100);

#if DEVICE_DMESG_BUFFER_SIZE > 0
    codal_dmesg_flush();
#endif
    uBit.sleep(500);

    int tMin = LONG_MAX;
    int tMax = 0;

    for ( int count = 0; true; count++)
    {
        if ( count % 10 == 0) uBit.display.image.setPixelValue( 4, 4, uBit.display.image.getPixelValue( 4, 4) ? 0 : 255);
        buffer[ 0] = count % 256;

        CODAL_TIMESTAMP t0 = system_timer_current_time_us();
        neopixel_send_buffer( uBit.io.pin[0], buffer, bufferSize);
        CODAL_TIMESTAMP t1 = system_timer_current_time_us();

        int tD = t1 - t0;
        DMESG( "%d", tD);
        if ( tMin > tD || tMax < tD)
        {
            if ( tMin > tD) tMin = tD;
            if ( tMax < tD) tMax = tD;
            output = output + ManagedString( (int) (t0 % INT_MAX)) + " " + ManagedString( tMin) + " " + ManagedString( tMax) + "\r\n";
            if ( stopTime && tD > stopTime)
            {
              uBit.sleep( 100);
#if DEVICE_DMESG_BUFFER_SIZE > 0
              codal_dmesg_flush();
#endif
              uBit.sleep( 1000);
              sendOutput();
              uBit.sleep( 1000);
              target_panic(777);
            }

        }

        if ( buttonA)
        {
            buttonA = false;
            sendOutput();
            uBit.sleep( 100);
        }

        if ( buttonB)
        {
            buttonB = false;
#if DEVICE_DMESG_BUFFER_SIZE > 0
            codal_dmesg_flush();
#endif
        }

        if ( sleep >= 0) uBit.sleep( sleep);
    }
}

int main() {
    uBit.init();
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,  MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    create_fiber( forever);
    release_fiber();
    return 0;
}
