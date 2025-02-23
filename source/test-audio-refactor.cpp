#include "MicroBit.h"
#include "StreamRecording.h"
#include "SerialStreamer.h"

MicroBit uBit;

int doRecord = 0;
int doRecordTwice = 0;
int doPlay   = 0;
int doStream = 0;

bool activateBeforeRecord = false;

bool analogActive = false;

int analog0 = 0;

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
    if ( activateBeforeRecord)
    {
        uBit.audio.activateMic();
        uBit.audio.mic->setStartDelay(4);
    }

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

void toggleAnalogue()
{
    analogActive = !analogActive;
    if (analogActive)
    {
      uBit.display.print( "A");
      analog0 = uBit.io.P2.getAnalogValue();
    }
    else
    {
      uBit.display.print( "a");
      uBit.adc.releaseChannel( uBit.io.P2);
    }
    uBit.sleep(20);
    uBit.display.clear();
}


void forever_analog()
{
    while (true) {
        if (analogActive)
        {
          analog0 = uBit.io.P2.getAnalogValue();
        }
        uBit.sleep(5000);
    }
}

void forever()
{
    initialise();

    while (true) {
        if ( doRecord)
        { 
            doRecord--;
            uBit.sleep(1000);
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
            uBit.display.print("P");
            DMESG("PLAY");
            play();
            DMESG("PLAY done");
            uBit.display.clear();
        }

        if ( doStream)
        { 
            doStream--; 
            uBit.display.print("S");
            uBit.serial.send("STREAM\n");
            streamAsync(); 
            uBit.serial.send("STREAM done\n");
            uBit.display.clear();
            DMESG( "analog0 %d", (int) analog0);
        }

        uBit.sleep(20);
    }
}


void onButtonA(MicroBitEvent e)
{
    activateBeforeRecord = false;
    doRecord++;
}

void onButtonB(MicroBitEvent e)
{
    activateBeforeRecord = true;
    doRecord++;
}

void onButtonAB(MicroBitEvent e)
{
}

void onButtonLogo(MicroBitEvent e)
{
    doStream++;
}

void onButtonP0(MicroBitEvent e)
{
    toggleAnalogue();
}

void onButtonP1(MicroBitEvent e)
{
}

int main() {
    uBit.init();

    uBit.io.P0.isTouched( TouchMode::Capacitative);
    uBit.io.P1.isTouched( TouchMode::Capacitative);

    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,   MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,   MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_AB,  MICROBIT_BUTTON_EVT_CLICK, onButtonAB);
    uBit.messageBus.listen( MICROBIT_ID_LOGO,       MICROBIT_BUTTON_EVT_CLICK, onButtonLogo);
    uBit.messageBus.listen( MICROBIT_ID_IO_P0,      MICROBIT_BUTTON_EVT_CLICK, onButtonP0);
    uBit.messageBus.listen( MICROBIT_ID_IO_P1,      MICROBIT_BUTTON_EVT_CLICK, onButtonP1);

    create_fiber( forever);
    create_fiber( forever_analog);

    release_fiber();
    return 0;
}
