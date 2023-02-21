#include "MicroBit.h"

MicroBit uBit;

void onButtonA(MicroBitEvent e)
{
    uBit.audio.activateMic();
}


void forever()
{
    while (true)
    {
      float soundLevel = uBit.audio.levelSPL->getValue();

      ManagedString str( (int) floor( 0.5 + soundLevel));
      uBit.serial.send( "level:" + str + "\n");
      uBit.sleep(200);
    }
}

int main()
{
    uBit.init();
    create_fiber( forever);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    release_fiber();
}