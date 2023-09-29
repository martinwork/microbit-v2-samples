#include "MicroBit.h"
#include "SerialStreamer.h"
#include "Tests.h"

MicroBit uBit;

int main() {
    uBit.init();

    SerialStreamer *streamer = new SerialStreamer(uBit.audio.processor->output, SERIAL_STREAM_MODE_BINARY);
    uBit.audio.requestActivation();
    uBit.audio.activateMic();
     
    while (true) {
        //RefCounted_alltimeunique();
        uBit.sleep(200);
    }
}
