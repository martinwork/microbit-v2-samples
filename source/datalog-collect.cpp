#include "MicroBit.h"


MicroBit uBit;

MicroBitUARTService *uart;

bool datalog_collect_running = false;

/**
 * Send the logged data to serial or BLE UART.
 * @param uart BLE uart service to send to. NULL to send to serial.
 */
void datalog_sendCSV(MicroBitUARTService *uart)
{
    bool useUART = uart && uart->getConnected();

    uint32_t dataStart;
    uint32_t dataEnd;
    uBit.log.getDataStartAndEnd( &dataStart, &dataEnd);

    uint32_t size    = dataEnd - dataStart;
    uint32_t address = dataStart;

    FSCache cache(uBit.flash, CONFIG_MICROBIT_LOG_CACHE_BLOCK_SIZE, 4);

    ManagedBuffer buffer(1024);
    ManagedString csv;

    while (size)
    {
        uint32_t block = buffer.length();
        if ( block > size)
            block = size;

        cache.read(address, &buffer[0], block);

        uint32_t iStart = 0;
        uint32_t iNext = 0;
        for ( uint32_t i = 0; i < block; i++)
        {
            if ( buffer[i] == '\n')
            {
                if ( iNext > iStart)
                {
                    ManagedString s( (const char *) &buffer[ iStart], iNext - iStart);
                    csv = csv + s;
                }
                csv = csv + "\r\n";
                if (useUART)
                    uart->send(csv);
                else
                    uBit.serial.send(csv);
                csv = ManagedString::EmptyString;
                iStart = i + 1;
            }
            iNext = i + 1;
        }
        if ( iStart < block && iNext > iStart)
        {
            ManagedString s( (const char *) &buffer[ iStart], iNext - iStart);
            csv = csv + s;
        }

        address += block;
        size -= block;
    }
    if ( csv.length())
    {
        csv = csv + "\r\n";
        if (useUART)
            uart->send(csv);
        else
            uBit.serial.send(csv);
    }
}


void datalog_collect()
{
    datalog_collect_running = true;
    datalog_sendCSV( uart);
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
