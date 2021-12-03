#include "MicroBit.h"


MicroBit uBit;

MicroBitUARTService *uart;

bool datalog_collect_running = false;

void datalog_collect()
{
    datalog_collect_running = true;
    uBit.log.sendCSV( uart);
    datalog_collect_running = false;
    release_fiber();
}


void onClickA( MicroBitEvent e)
{
    if ( !datalog_collect_running)
    {
        datalog_collect_running = true;
        create_fiber(datalog_collect);
    }
}


void display()
{
    uBit.display.scroll( "CONNECT SERIAL/BLE UART", 100);

    while (true)
    {
        if ( datalog_collect_running)
            uBit.display.scroll( "STREAMING...", 100);
        else
            uBit.display.scroll( "A=STREAM CSV", 100);
        uBit.sleep(1000);
    }
}


int main()
{
    uBit.init();

    uBit.serial.setRxBufferSize(240);
    uBit.serial.setTxBufferSize(240);

    uart = new MicroBitUARTService( *uBit.ble, 240, 240);

    uBit.messageBus.listen(DEVICE_ID_BUTTON_A, DEVICE_BUTTON_EVT_CLICK, onClickA);

    create_fiber( display);
    release_fiber();
}
