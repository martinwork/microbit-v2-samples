#include "MicroBit.h"

MicroBit uBit;


int microbit_microphone_sound_level();

int 
main()
{
    uBit.init();

    while(1)
    {
      if ( uBit.buttonA.isPressed())
      {
        static int level0 = -1;
        if ( level0 < 0)
          level0 = microbit_microphone_sound_level();
        uBit.serial.send( ManagedString(level0) + ", " + ManagedString(microbit_microphone_sound_level()) + "\n");

        uBit.io.runmic.setDigitalValue(1);
        uBit.io.runmic.setHighDrive(true);

        if ( microbit_microphone_sound_level() > 10)
          uBit.display.printChar('2');
        else if ( level0 > 0)
          uBit.display.printChar('1');
        else
          uBit.display.printChar('0');
      }
      else
      {
        uBit.io.runmic.setDigitalValue(0);
        uBit.io.runmic.setHighDrive(false);
        uBit.display.printChar('?');
      }
    }
}



/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#define SOUND_LEVEL_MAXIMUM (20000)

static NRF52ADCChannel *mic = NULL;
static StreamNormalizer *processor = NULL;
static LevelDetector *level = NULL;

void microbit_hal_microphone_init(void) {
    if (mic == NULL) {
        mic = uBit.adc.getChannel(uBit.io.microphone);
        mic->setGain(7, 0);

        processor = new StreamNormalizer(mic->output, 0.05, true, DATASTREAM_FORMAT_8BIT_SIGNED);
        level = new LevelDetector(processor->output, 600, 200);

        uBit.io.runmic.setDigitalValue(1);
        uBit.io.runmic.setHighDrive(true);
    }
}

int microbit_hal_microphone_get_level(void) {
    if (level == NULL) {
        return -1;
    } else {
        int l = level->getValue();
        l = min(255, l * 255 / SOUND_LEVEL_MAXIMUM);
        return l;
    }
}


void microphone_init(void) {
    microbit_hal_microphone_init();
}


int microbit_microphone_sound_level()
{
    microphone_init();
    return microbit_hal_microphone_get_level();
}

