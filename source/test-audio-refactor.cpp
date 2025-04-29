#include "MicroBit.h"

MicroBit uBit;

int decibels2makecode( int db)
{
  db = max( 52, min( db, 120));
  db = (db - 52) * 255 / ( 120 - 52);
  db = max( 0, min( db, 255));
  return db;
}

void forever()
{
    while (true)
    {
        uBit.display.scroll( ManagedString( (int) decibels2makecode( uBit.audio.levelSPL->getValue())));
        uBit.sleep(20);
    }
}

int main()
{
    uBit.init();
    create_fiber( forever);
    release_fiber();
}

//#include "MicroBit.h"
//#include "SerialStreamer.h"
//
//MicroBit uBit;
//
//int main() {
//    uBit.init();
//
//    static SplitterChannel *splitterChannel = uBit.audio.splitter->createChannel();
//    SerialStreamer *streamer = new SerialStreamer(*splitterChannel, SERIAL_STREAM_MODE_DECIMAL);
//    //uBit.audio.requestActivation();
//    //uBit.audio.activateMic();
//    DMESG("main calling dataWanted");
//    splitterChannel->dataWanted( DATASTREAM_WANTED);
//    while (true) {
//        uBit.sleep(1000);
//    }
//}

//#include "MicroBit.h"
//#include "SerialStreamer.h"
//
//MicroBit uBit;
//
//int main() {
//    uBit.init();
//
//    static SplitterChannel *splitterChannel = uBit.audio.splitter->createChannel();
//    // splitterChannel->requestSampleRate( 11000 ); // <-- This is only needed for builds from the main branch
//    SerialStreamer *streamer = new SerialStreamer(*splitterChannel, SERIAL_STREAM_MODE_DECIMAL);
//    uBit.audio.requestActivation();
//    uBit.audio.activateMic();
//
//    while (true) {
//        uBit.sleep(1000);
//    }
//}

//#include "MicroBit.h"
//#include "SerialStreamer.h"
//
//MicroBit uBit;
//
//int main() {
//    uBit.init();
//
//    SerialStreamer *streamer = new SerialStreamer(uBit.audio.processor->output, SERIAL_STREAM_MODE_DECIMAL);
//    uBit.audio.requestActivation();
//    uBit.audio.activateMic();
//
//    while (true) {
//        uBit.sleep(1000);
//    }
//}

//#include "MicroBit.h"
//#include "StreamRecording.h"
//#include "SerialStreamer.h"
//
//MicroBit uBit;
//
//void forever()
//{
//    SplitterChannel *splitterChannel  = uBit.audio.splitter->createChannel();
//    StreamRecording *recording        = new StreamRecording(*splitterChannel);
//    SerialStreamer  *streamer         = new SerialStreamer(*recording, SERIAL_STREAM_MODE_DECIMAL);
//
//    while (true)
//    {
//        uBit.display.image.setPixelValue( 0, 0, uBit.display.image.getPixelValue( 0, 0) ? 0 : 255);
//
//        uBit.display.print("R");
//        DMESG("PAUSE");
//        uBit.sleep(1000);
//        DMESG("RECORD");
//
//        // on demand activateMic doesn't work after an analogue read of a pin
//        // calling activateMic in advance stops it hanging
//        //uBit.audio.activateMic();
//        //uBit.audio.mic->setStartDelay(4);
//
//        recording->recordAsync();
//        while ( recording->isRecording() && recording->duration( recording->getSampleRate()) < 0.1)
//        {
//            uBit.display.image.setPixelValue( 4, 0, uBit.display.image.getPixelValue( 4, 0) ? 0 : 255);
//            uBit.sleep(0);
//        }
//        recording->stop();
//
//        DMESG("RECORD done");
//        uBit.display.clear();
//
//        DMESG("STREAM");
//        uBit.sleep(100);
//        uBit.display.print("A");
//        recording->connect(*streamer);
//        DMESG("playAsync");
//        recording->playAsync();   // works like sync
//        DMESG("playAsync done");
//        uBit.sleep(500);
//        DMESG("play");
//        uBit.sleep(100);
//        uBit.display.print("P");
//        recording->play();         // play() hangs
//        DMESG("STREAM done");
//
//        uBit.sleep(100);
//    }
//}
//
//int main() {
//    uBit.init();
//    create_fiber( forever);
//    release_fiber();
//    return 0;
//}



//#include "MicroBit.h"
//#include "StreamRecording.h"
//#include "SerialStreamer.h"
//
//MicroBit uBit;
//
//void forever()
//{
//    SplitterChannel *splitterChannel  = uBit.audio.splitter->createChannel();
//    StreamRecording *recording        = new StreamRecording(*splitterChannel);
//    SerialStreamer  *streamer         = new SerialStreamer(*recording, SERIAL_STREAM_MODE_DECIMAL);
//
//    while (true)
//    {
//        uBit.display.image.setPixelValue( 0, 0, uBit.display.image.getPixelValue( 0, 0) ? 0 : 255);
//        uBit.sleep(100);
//
//        if ( uBit.buttonB.isPressed())
//        {
//            uBit.display.print("B");
//            DMESG("PAUSE");
//            uBit.sleep(1000);
//            uBit.display.print("R");
//            DMESG("RECORD");
//
//            recording->recordAsync();
//            while ( recording->isRecording() && recording->duration( recording->getSampleRate()) < 0.1)
//            {
//                int analog = uBit.io.P2.getAnalogValue();
//                DMESG("analog %d", (int) analog);
//
//                uBit.display.image.setPixelValue( 4, 0, uBit.display.image.getPixelValue( 4, 0) ? 0 : 255);
//                uBit.sleep(10);
//            }
//            recording->stop();
//            uBit.adc.releaseChannel( uBit.io.P2);
//
//            DMESG("RECORD done");
//            uBit.display.clear();
//        }
//    }
//}
//
//
//int main() {
//    uBit.init();
//    create_fiber( forever);
//    release_fiber();
//    return 0;
//}
//
//
//int doRecord = 0;
//int doRecordTwice = 0;
//int doPlay   = 0;
//int doStream = 0;
//
//bool activateBeforeRecord = false;
//
//bool analogActive = false;
//
//int analog0 = 0;
//
//static SplitterChannel *splitterChannel = NULL;
//static StreamRecording *recording = NULL;
//static MixerChannel *mixerChannel = NULL;
//static SerialStreamer *streamer = NULL;
//
//void initialise()
//{
//    if ( splitterChannel)
//        return;
//
//    DMESG("INITIALISE");
//
//    splitterChannel = uBit.audio.splitter->createChannel();
//
//    //splitterChannel->requestSampleDropRate(1);
//
//    recording = new StreamRecording(*splitterChannel);
//
//    mixerChannel = uBit.audio.mixer.addChannel(*recording, recording->getSampleRate());
//
//    streamer = new SerialStreamer(*recording, SERIAL_STREAM_MODE_DECIMAL);
//
//    MicroBitAudio::requestActivation();
//    mixerChannel->setVolume(75.0);
//    uBit.audio.mixer.setVolume(1023);
//    
//    DMESG("Recording rate: %d", (int)recording->getSampleRate());
//    DMESG("Mixer rate: %d", (int)uBit.audio.mixer.getSampleRate());
//}
//
//void record()
//{
//    if ( activateBeforeRecord)
//    {
//        uBit.audio.activateMic();
//        uBit.audio.mic->setStartDelay(4);
//    }
//
//    recording->recordAsync();
//    while ( recording->isRecording() && recording->duration( recording->getSampleRate()) < 0.1)
//    {
//        uBit.sleep(0);
//    }
//    recording->stop();
//    DMESG("Recording length:   %d bytes", (int) recording->length());
//    DMESG("Recording rate:     %d Hz", (int) recording->getSampleRate());
//    DMESG("Recording duration: %d ms", (int) ( 1000 * recording->duration( recording->getSampleRate())));
//}
//
//void play()
//{
//    recording->connect(*mixerChannel);
//    recording->play();
//}
//
//void stream()
//{
//    recording->connect(*streamer);
//    recording->play();
//}
//
//void streamAsync()
//{
//    recording->connect(*streamer);
//    recording->playAsync();
//}
//
//void toggleAnalogue()
//{
//    analogActive = !analogActive;
//    if (analogActive)
//    {
//      uBit.display.print( "A");
//      analog0 = uBit.io.P2.getAnalogValue();
//    }
//    else
//    {
//      uBit.display.print( "a");
//      uBit.adc.releaseChannel( uBit.io.P2);
//    }
//    uBit.sleep(20);
//    uBit.display.clear();
//}
//
//
//void forever_analog()
//{
//    while (true) {
//        if (analogActive)
//        {
//          //analog0 = uBit.io.P2.getAnalogValue();
//        }
//        uBit.sleep(5000);
//    }
//}
//
//void forever()
//{
//    initialise();
//
//    while (true) {
//        if ( doRecord)
//        { 
//            doRecord--;
//            uBit.sleep(1000);
//            uBit.display.print("R");
//            DMESG("RECORD");
//            record();
//            DMESG("RECORD done");
//            uBit.display.clear();
//        }
//
//        if ( doRecordTwice)
//        { 
//            doRecordTwice--;
//            uBit.sleep(500);
//            uBit.display.print("T");
//            DMESG("RECORD TWICE");
//            record();
//            record();
//            DMESG("RECORD TWICE done");
//            uBit.display.clear();
//        }
//
//        if ( doPlay)
//        { 
//            doPlay--;
//            uBit.display.print("P");
//            DMESG("PLAY");
//            play();
//            DMESG("PLAY done");
//            uBit.display.clear();
//        }
//
//        if ( doStream)
//        { 
//            doStream--; 
//            uBit.display.print("S");
//            DMESG("STREAM\n");
//            streamAsync(); 
//            DMESG("STREAM done\n");
//            uBit.display.clear();
//            DMESG( "analog0 %d", (int) analog0);
//        }
//
//        uBit.sleep(20);
//    }
//}
//
//
//void onButtonA(MicroBitEvent e)
//{
//    activateBeforeRecord = false;
//    doRecord++;
//}
//
//void onButtonB(MicroBitEvent e)
//{
//    activateBeforeRecord = true;
//    doRecord++;
//}
//
//void onButtonAB(MicroBitEvent e)
//{
//}
//
//void onButtonLogo(MicroBitEvent e)
//{
//    doStream++;
//}
//
//void onButtonP0(MicroBitEvent e)
//{
//    toggleAnalogue();
//}
//
//void onButtonP1(MicroBitEvent e)
//{
//}
//
//int main() {
//    uBit.init();
//
//    uBit.io.P0.isTouched( TouchMode::Capacitative);
//    uBit.io.P1.isTouched( TouchMode::Capacitative);
//
//    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A,   MICROBIT_BUTTON_EVT_CLICK, onButtonA);
//    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B,   MICROBIT_BUTTON_EVT_CLICK, onButtonB);
//    uBit.messageBus.listen( MICROBIT_ID_BUTTON_AB,  MICROBIT_BUTTON_EVT_CLICK, onButtonAB);
//    uBit.messageBus.listen( MICROBIT_ID_LOGO,       MICROBIT_BUTTON_EVT_CLICK, onButtonLogo);
//    uBit.messageBus.listen( MICROBIT_ID_IO_P0,      MICROBIT_BUTTON_EVT_CLICK, onButtonP0);
//    uBit.messageBus.listen( MICROBIT_ID_IO_P1,      MICROBIT_BUTTON_EVT_CLICK, onButtonP1);
//
//    create_fiber( forever);
//    create_fiber( forever_analog);
//
//    release_fiber();
//    return 0;
//}


