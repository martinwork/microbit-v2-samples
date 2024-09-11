#include "MicroBit.h"
#include "StreamRecording.h"

MicroBit uBit;

const int SAMPLE_RATE = 7812;
const int BUFFERS_LEN = 10;
const int BUFFERS_SIZE = 128;
uint8_t buffers[BUFFERS_LEN][BUFFERS_SIZE];

class MyStreamRecording : public DataSink {
public:
    SplitterChannel *upStream;
    uint8_t *dest;
    int dest_pos_ptr;
    size_t dest_max;
    bool request_stop;

    MyStreamRecording(SplitterChannel *source) : upStream(source) { }
    virtual ~MyStreamRecording() { }

    float getSampleRate() {
        return this->upStream->getSampleRate();
    }
    bool isRecording() {
        return upStream->isConnected();
    }
    void stopRecording() {
        this->request_stop = true;
    }
    void recordAsync(uint8_t *buf, size_t max_len) {
        this->dest = buf;
        this->dest_max = max_len;
        this->dest_pos_ptr = 0;
        this->request_stop = false;

        upStream->connect(*this);
    }
    virtual int pullRequest() {
        uint8_t *pull_buf = this->dest + this->dest_pos_ptr;
        size_t n = this->dest_max - this->dest_pos_ptr;

        if (n > 0) {
            n = this->upStream->pullInto(pull_buf, n) - pull_buf;
            DMESGN( "%d ", (int)(uint8_t)pull_buf[0]);
        }

        if (n == 0 || this->request_stop) {
            this->upStream->disconnect();
            this->request_stop = false;
        } else {
            // Convert signed 8-bit to unsigned 8-bit data.
            for (size_t i = 0; i < n; ++i) {
                pull_buf[i] += 128;
            }
            this->dest_pos_ptr += n;
        }

        return DEVICE_OK;
    }
};

int main() {
    uBit.init();

    SplitterChannel *splitterChannel = uBit.audio.splitter->createChannel();
    splitterChannel->requestSampleRate(SAMPLE_RATE);
    MyStreamRecording *recording = new MyStreamRecording(splitterChannel);

    memset(buffers, 128, sizeof(buffers));

    while (true) {
        if (uBit.buttonA.isPressed()) {
            // Record data into buffers
            DMESG( "");
            uBit.display.print("R");
            uBit.sleep(500);
            for (int i = 0; i < BUFFERS_LEN; ++i) {
                DMESG( "");
                recording->recordAsync(buffers[i], BUFFERS_SIZE);
                while (recording->isRecording()) {
                    uBit.sleep(1);
                }
            }
        }
        if (uBit.buttonB.isPressed()) {
            // Print buffers to serial in CSV format
            uBit.display.print("P");
            for (int i = 0; i < BUFFERS_SIZE; ++i) {
                for (int j = 0; j < BUFFERS_LEN - 1; ++j) {
                    uBit.serial.printf("%d, ", buffers[j][i]);
                }
                uBit.serial.printf("%d\n", buffers[BUFFERS_LEN - 1][i]);
            }
        }
        uBit.display.clear();
        uBit.sleep(100);
    }
}