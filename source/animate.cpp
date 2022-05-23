#include "MicroBit.h"


MicroBit uBit;


MicroBitImage img("255 0 0 0 0 0 0 0 0 255\n"
                  "0 255 0 0 0 0 0 0 255 0\n"
                  "0 0 255 0 0 0 0 255 0 0\n"
                  "0 0 0 255 0 0 255 0 0 0\n"
                  "0 0 0 0 255 255 0 0 0 0\n");

int stride = 1;


void onButtonA(MicroBitEvent e)
{
    stride = -1;
    uBit.display.stopAnimation();
}


void onButtonB(MicroBitEvent e)
{
    stride = 1;
    uBit.display.stopAnimation();
}


void forever()
{
    while ( true)
    {
      uBit.display.stopAnimation();
      uBit.display.clear();
      uBit.display.animate(img, 500, stride, MICROBIT_DISPLAY_ANIMATE_DEFAULT_POS, 0);

      uBit.display.stopAnimation();
      uBit.display.clear();
      uBit.display.animate(img, 500, stride, 2, 0);
    }
}


int  main()
{
    uBit.init();

    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,  MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,  MICROBIT_BUTTON_EVT_CLICK, onButtonB);

    create_fiber( forever);

    release_fiber();
}
