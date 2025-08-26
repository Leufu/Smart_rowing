//#include <esp32-hal-bt.c>

#include <NimBLEDevice.h>
#define	size_of_struct 12000
#define  chunk_size 128


const char* matrixUUID= "180F";
//BLEService sensorService(matrixUUID);
//BLEStringCharacteristic sensorCharacteristic(matrixUUID,BLERead | BLENotify,128);

// Creation of the server, service and characteristic for sending data
static NimBLEServer* pServer = nullptr;
static NimBLEService* pSensorService = nullptr;
static NimBLECharacteristic* pSensorCharacteristic = nullptr;






// Se next function start the BLE service, createds the Server, service and characteristics for sending the data
//
//
void Init_BLE()
{	

  //NimBLEDevice::init();
  NimBLEDevice::init("Peripheral_foot");
  //NimBLEAdvertising::enableScanResponse()
	//NimBLEAdvertising::setName("name1");
		//NimBLEAdvertisementData::setName("name2");
  NimBLEDevice::setMTU(256); //setMTU after init the BLEDEVICE XDD else crash 
	//if (!NimBLEDevice::setDeviceName("Sick-FSR-insole")) 
	////{Serial.print("Name not changed");}
  pServer = NimBLEDevice::createServer();

  // Crear servicio
  pSensorService = pServer->createService(matrixUUID);

  // Crear característica con notificación
  pSensorCharacteristic = pSensorService->createCharacteristic(
    matrixUUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
  );

  // Valor inicial
  pSensorCharacteristic->setValue("0");

  // Iniciar el servicio
  pSensorService->start();

  // Configurar publicidad
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pSensorService->getUUID());
  ////pAdvertising->setScanResponse(true);
  pAdvertising->enableScanResponse(true);
  pAdvertising->start();

  pAdvertising->enableScanResponse(true);
  //pAdvertising->setScanResponseData(true);
  Serial.println("FSR (Sending Data) - NimBLE iniciado");
  Serial.printf("MTU = %i",NimBLEDevice::getMTU() );

}



