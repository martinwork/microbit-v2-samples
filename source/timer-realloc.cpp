#include "MicroBit.h"

MicroBit uBit;

uint16_t source = 60000;

void onTimer(MicroBitEvent e)
{
    DMESG( "%u:onTimer source %d value %d ", (unsigned int) system_timer_current_time_us(), (int) e.source, (int) e.value);
}

int 
main()
{
    uBit.init();

    uBit.messageBus.listen( source, DEVICE_EVT_ANY, onTimer);

    int numTimers = CODAL_TIMER_DEFAULT_EVENT_LIST_SIZE * 3;

    for ( int i = 1; i <= numTimers; i++)
    {
        system_timer_event_after_us( 1000000 + i * 200000, source, i);
    }
    
    DMESG( "%u:created %d timers", (unsigned int) system_timer_current_time_us(), numTimers);

    while(1)
        uBit.sleep(1000);
}