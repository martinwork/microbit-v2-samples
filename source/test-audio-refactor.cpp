#include "MicroBit.h"
#include "StreamRecording.h"
#include "SerialStreamer.h"

MicroBit uBit;

int doRecord = 0;
int doRecordTwice = 0;
int doPlay   = 0;
int doStream = 0;

static SplitterChannel *splitterChannel = NULL;
static StreamRecording *recording = NULL;
static MixerChannel *mixerChannel = NULL;
static SerialStreamer *streamer = NULL;

void initialise()
{
    if ( splitterChannel)
        return;

    DMESG("INITIALISE");

    splitterChannel = uBit.audio.splitter->createChannel();

    //splitterChannel->requestSampleDropRate(1);

    recording = new StreamRecording(*splitterChannel);

    mixerChannel = uBit.audio.mixer.addChannel(*recording, recording->getSampleRate());

    streamer = new SerialStreamer(*recording, SERIAL_STREAM_MODE_DECIMAL);

    MicroBitAudio::requestActivation();
    mixerChannel->setVolume(75.0);
    uBit.audio.mixer.setVolume(1023);
    
    DMESG("Recording rate: %d", (int)recording->getSampleRate());
    DMESG("Mixer rate: %d", (int)uBit.audio.mixer.getSampleRate());
}

void record()
{
    recording->recordAsync();
    while ( recording->isRecording() && recording->duration( recording->getSampleRate()) < 0.1)
    {
        uBit.sleep(0);
    }
    recording->stop();
    DMESG("Recording length:   %d bytes", (int) recording->length());
    DMESG("Recording rate:     %d Hz", (int) recording->getSampleRate());
    DMESG("Recording duration: %d ms", (int) ( 1000 * recording->duration( recording->getSampleRate())));
}

void play()
{
    recording->connect(*mixerChannel);
    recording->play();
}

void stream()
{
    recording->connect(*streamer);
    recording->play();
}

void streamAsync()
{
    recording->connect(*streamer);
    recording->playAsync();
}

void forever()
{
    while (true) {
        if ( doRecord)
        { 
            doRecord--;
            uBit.sleep(500);
            initialise();
            uBit.display.print("R");
            DMESG("RECORD");
            record();
            DMESG("RECORD done");
            uBit.display.clear();
        }

        if ( doRecordTwice)
        { 
            doRecordTwice--;
            uBit.sleep(500);
            initialise();
            uBit.display.print("T");
            DMESG("RECORD TWICE");
            record();
            record();
            DMESG("RECORD TWICE done");
            uBit.display.clear();
        }

        if ( doPlay)
        { 
            doPlay--;
            initialise();
            uBit.display.print("P");
            DMESG("PLAY");
            play();
            DMESG("PLAY done");
            uBit.display.clear();
        }

        if ( doStream)
        { 
            doStream--; 
            initialise();
            uBit.display.print("S");
            uBit.serial.send("STREAM\n");
            streamAsync(); 
            uBit.serial.send("STREAM done\n");
            uBit.display.clear();
        }

        uBit.sleep(20);
    }
}

void onButtonA(MicroBitEvent e)
{
    doRecord++;
}

void onButtonB(MicroBitEvent e)
{
    doRecordTwice++;
}

void onButtonAB(MicroBitEvent e)
{
    doPlay++;
}

void onButtonLogo(MicroBitEvent e)
{
    doStream++;
}

int main() {
    uBit.init();

    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,   MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,   MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_AB,  MICROBIT_BUTTON_EVT_CLICK, onButtonAB);
    uBit.messageBus.listen( MICROBIT_ID_LOGO,       MICROBIT_BUTTON_EVT_CLICK, onButtonLogo);

    create_fiber( forever);

    release_fiber();
    return 0;
}
