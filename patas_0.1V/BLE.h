// UUID Heart Rate Sensor 0x0340 to 0x037F Service
// UUID Health Sensor Features 0x2BF3 Characteristic

//////////////BLE Config/////////////////
#include <ArduinoBLE.h>
#define SERVICE_UUID "0340"
#define CHAR_UUID_ADS "2BF3"

BLEService ADS_service(SERVICE_UUID);
BLEIntCharacteristic ADS_A0_char(CHAR_UUID_ADS, BLENotify| BLERead);// |BLEIndicate

////////////////////////////////////////
