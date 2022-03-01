#include "MicroBit.h"
#include "MicroBitUtilityService.h"


MicroBit uBit;

MicroBitUARTService *uart;

bool log_collect_running = false;

/**
 * Send the logged data to serial or BLE UART.
 * @param uart BLE uart service to send to. NULL to send to serial.
 */
void log_sendCSV(MicroBitUARTService *uart)
{
    bool useUART = uart && uart->getConnected();

    uint32_t length = uBit.log.getDataLength( DataFormat::CSV);
    uint32_t index  = 0;

    ManagedBuffer buffer(CONFIG_MICROBIT_LOG_CACHE_BLOCK_SIZE);

    while (length)
    {
        uint32_t block = min(buffer.length(),length);

        if (DEVICE_OK != uBit.log.readData( DataFormat::CSV, index, &buffer[0], block))
            break;

        if (useUART)
            uart->send(&buffer[0], block);
        else
            uBit.serial.send(&buffer[0], block);

        index += block;
        length -= block;
    }
}


void log_collect()
{
    log_collect_running = true;
    log_sendCSV( uart);
    log_collect_running = false;
    release_fiber();
}


void onClickA( MicroBitEvent e)
{
    if ( !log_collect_running)
    {
        log_collect_running = true;
        create_fiber(log_collect);
    }
}


void display()
{
    uBit.display.scroll( "CONNECT SERIAL/BLE UART", 100);

    while (true)
    {
        if ( log_collect_running)
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

    MicroBitUtilityService::createShared( *uBit.ble, uBit.messageBus, uBit.storage, uBit.log);
  
    uart = new MicroBitUARTService( *uBit.ble, 240, 240);

    uBit.messageBus.listen(DEVICE_ID_BUTTON_A, DEVICE_BUTTON_EVT_CLICK, onClickA);

    create_fiber( display);
    release_fiber();
}
