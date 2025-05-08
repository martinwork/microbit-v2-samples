#include "MicroBit.h"
MicroBit uBit;

void onButtonA(MicroBitEvent e)
{
  bool isTouched1 = uBit.io.logo.isTouched();
  uBit.sleep(50);
  bool isTouched2 = uBit.io.logo.isTouched();
  
  uBit.display.print( isTouched1 ? 'Y' : 'N');
  uBit.sleep(500);
  uBit.display.clear();
  uBit.sleep(100);
  uBit.display.print( isTouched2 ? 'Y' : 'N');
  uBit.sleep(500);
  uBit.display.clear();
}

void onButtonB(MicroBitEvent e)
{
  bool isTouched1 = uBit.logo.isPressed();
  uBit.sleep(50);
  bool isTouched2 = uBit.logo.isPressed();
  
  uBit.display.print( isTouched1 ? 'Y' : 'N');
  uBit.sleep(500);
  uBit.display.clear();
  uBit.sleep(100);
  uBit.display.print( isTouched2 ? 'Y' : 'N');
  uBit.sleep(500);
  uBit.display.clear();
}

void forever()
{
    while (true)
    {
      uBit.sleep(1000);
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