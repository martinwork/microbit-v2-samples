#ifndef BLE_HF2_SERVICE_H
#define BLE_HF2_SERVICE_H

#include "MicroBit.h"

#ifndef MICROBIT_CODAL
#ifdef CODAL_CONFIG_H
#define MICROBIT_CODAL 1
#else
#define MICROBIT_CODAL 0
#endif
#endif

#if MICROBIT_CODAL
#include "MicroBitBLEManager.h"
#include "MicroBitBLEService.h"
#else
#include "ble/BLE.h"
#endif

#include "MicroBitThermometer.h"
#include "EventModel.h"

#define HF2_ID 9501

#define BLEHF2_FLAG_SERIAL_OUT 0x80
#define BLEHF2_FLAG_SERIAL_ERR 0xC0
#define BLEHF2_DATA_LENGTH 19

struct BLEHF2Packet {
  uint8_t command;
  uint8_t data[BLEHF2_DATA_LENGTH];
};


#if MICROBIT_CODAL

class BLEHF2Service : public MicroBitBLEService
{
    public:

    /**
      * Constructor.
      * Create a representation of the TemperatureService
      * @param _ble The instance of a BLE device that we're running on.
      */
    BLEHF2Service(BLEDevice &_ble);

    /**
    * Sends text
    */
    void sendSerial(const char *data, int len, bool isError);

    private:

    // Bluetooth stack we're running on.
    BLEDevice &ble;

    // memory for buffers.
    BLEHF2Packet txCharacteristicMessage;
    
    // Index for each charactersitic in arrays of handles and UUIDs
    typedef enum mbbs_cIdx
    {
        mbbs_cIdxMESSAGE,
        mbbs_cIdxCOUNT
    } mbbs_cIdx;
    
    // UUIDs for our service and characteristics
    static const uint8_t  service_base_uuid[ 16];
    static const uint8_t  char_base_uuid[ 16];
    static const uint16_t serviceUUID;
    static const uint16_t charUUID[ mbbs_cIdxCOUNT];
    
    // Data for each characteristic when they are held by Soft Device.
    MicroBitBLEChar      chars[ mbbs_cIdxCOUNT];

    public:
    
    int              characteristicCount()          { return mbbs_cIdxCOUNT; };
    MicroBitBLEChar *characteristicPtr( int idx)    { return &chars[ idx]; };
};


#else //MICROBIT_CODAL


// UUIDs for our service and characteristics
extern const uint8_t  BLEHF2ServiceUUID[];
extern const uint8_t  BLEHF2TxCharacteristicUUID[];

class BLEHF2Service
{
    public:

    /**
      * Constructor.
      * Create a representation of the TemperatureService
      * @param _ble The instance of a BLE device that we're running on.
      */
    BLEHF2Service(BLEDevice &_ble);

    /**
    * Sends text
    */
    void sendSerial(const char *data, int len, bool isError); 

    private:

    // Bluetooth stack we're running on.
    BLEDevice &ble;

    // memory for buffers.
    BLEHF2Packet txCharacteristicMessage;

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t txCharacteristicHandle;
};


#endif //MICROBIT_CODAL

#endif
