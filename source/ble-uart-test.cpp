#include "MicroBit.h"
#include "MicroBitUARTService.h"


MicroBit uBit;
MicroBitUARTService* uart;

//ManagedString msg = R"(abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz)";
ManagedString alpha = "abcdefghijklmnopqrstuvwxyz\n";
ManagedString msg = "  0" + alpha + "  1" + alpha + "  2" + alpha + "  3" + alpha + "  4" + alpha + "  5" + alpha + "  6" + alpha + "  7" + alpha + "  8" + alpha + "  9" + alpha;


int uartSend(ManagedString s)
{
  // default mode is SYNC_SLEEP
  // return value is int, to allow for error codes
  int bytes = uart->send(s);
  DMESG( "uartSend bytes = %d", bytes);
  return bytes;
}

int uartSendAsync(ManagedString s)
{
    int length = s.length();
    int sent = 0;

    while ( sent < length)
    {
        int bytes = uart->send( s.substring( sent, length - sent), ASYNC);
        if ( bytes > 0)
        {
            DMESG( "uartSendAsync bytes = %d", bytes);
        }
        if ( bytes < 0) return bytes;
        sent += bytes;
    }
    return sent;
}

void onClickB(MicroBitEvent) 
{
    int sent;
    sent = uartSend( msg);
    DMESG( "onClickB sent = %d", sent);
    sent = uartSend("\n");
    DMESG( "onClickB sent = %d", sent);
}

void onClickA(MicroBitEvent) 
{
    int sent;
    sent = uartSendAsync(msg);
    DMESG( "onClickA sent = %d", sent);
    sent = uartSendAsync("\n");
    DMESG( "onClickA sent = %d", sent);
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
    uart = new MicroBitUARTService(*uBit.ble, 90, 90);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onClickB);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onClickA);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
