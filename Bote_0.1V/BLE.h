// UUID Heart Rate Sensor 0x0340 to 0x037F Service
// UUID Health Sensor Features 0x2BF3 Characteristic

//////////////BLE Config/////////////////
#include <ArduinoBLE.h>
#define SERVICE_UUID "0340"
#define CHAR_UUID_ADS "2BF3"
void printData(const unsigned char data[], int length);
void exploreService(BLEService service);
void exploreCharacteristic(BLECharacteristic characteristic);
void exploreDescriptor(BLEDescriptor descriptor);
void explorerPeripheral(BLEDevice peripheral);
void printDataDEC(const unsigned char data[], int length);
//BLEService ADS_service(SERVICE_UUID);
//BLEFloatCharacteristic ADS_A0_char(CHAR_UUID_ADS, BLENotify| BLERead);// |BLEIndicate

////////////////////////////////////////

void printData(const unsigned char data[], int length) {
  Serial.print(length);
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];
    if (b < 16) {
      Serial.print("0");
    }

    Serial.print(b, HEX);
  }
}
void printData_le(const unsigned char data[], int length) {
  Serial.print(length); Serial.print(" ");
  String s="";
  for (int i = length-1; i >=0; i--) {
    unsigned char b = data[i];
    s=s+b;
    if (b < 16) {
      Serial.print("0");
    }
    Serial.print(" ");
    Serial.print(b, HEX);
    
  }
  Serial.print(" ");
  Serial.print(s);
}
void printData_le_DEC(const unsigned char data[], int length) {
  if (length < 4) {
    Serial.println("No hay suficientes datos.");
    return; // Asegúrate de que hay suficientes bytes para un entero de 32 bits
  }

  // Interpretar los primeros 4 bytes como un entero little-endian
  uint32_t num = 0;
  for (int i = 0; i < length; i++) { // Asegúrate de que no lees más bytes de los que tiene data si data tiene menos de 4 bytes
    num |= (uint32_t)data[i] << (8 * i);
  }

  //Serial.print("Número interpretado: ");
  Serial.println(int(num));

  // Aquí `num` contiene el entero interpretado de los bytes
}
void printDataDEC(const unsigned char data[], int length) 
{
  String s="";
  for (int i = 0; i < length; i++) {
    s= s + data[i];
    //Serial.print(b);
  }
  Serial.print(s);
  /*
  for (int i = 0; i < length; i++) {
  unsigned char b = data[i];

  // Enviar el byte en formato decimal
  Serial.print(b, DEC);

  // Para mejorar la legibilidad, puedes agregar un espacio entre cada número
  if (i < length - 1) {Serial.print(" ");}
  }
  Serial.println(); // Añadi
  */

  
}
void explorerPeripheral(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // read and print device name of peripheral
  Serial.println();
  Serial.print("Device name: ");
  Serial.println(peripheral.deviceName());
  Serial.print("Appearance: 0x");
  Serial.println(peripheral.appearance(), HEX);
  Serial.println();

  // loop the services of the peripheral and explore each
  for (int i = 0; i < peripheral.serviceCount(); i++) {
    BLEService service = peripheral.service(i);

    exploreService(service);
  }

  Serial.println();

  // we are done exploring, disconnect
  Serial.println("Disconnecting ...");
  peripheral.disconnect();
  Serial.println("Disconnected");
}

void exploreService(BLEService service) {
  // print the UUID of the service
  Serial.print("Service ");
  Serial.println(service.uuid());

  // loop the characteristics of the service and explore each
  for (int i = 0; i < service.characteristicCount(); i++) {
    BLECharacteristic characteristic = service.characteristic(i);

    exploreCharacteristic(characteristic);
  }
}

void exploreCharacteristic(BLECharacteristic characteristic) {
  // print the UUID and properties of the characteristic
  Serial.print("\tCharacteristic ");
  Serial.print(characteristic.uuid());
  Serial.print(", properties 0x");
  Serial.print(characteristic.properties(), HEX);

  // check if the characteristic is readable
  if (characteristic.canRead()) {
    // read the characteristic value
    characteristic.read();

    if (characteristic.valueLength() > 0) {
      // print out the value of the characteristic
      Serial.print(", value 0x");
      printData(characteristic.value(), characteristic.valueLength());
    }
  }
  Serial.println();

  // loop the descriptors of the characteristic and explore each
  for (int i = 0; i < characteristic.descriptorCount(); i++) {
    BLEDescriptor descriptor = characteristic.descriptor(i);

    exploreDescriptor(descriptor);
  }
}

void exploreDescriptor(BLEDescriptor descriptor) {
  // print the UUID of the descriptor
  Serial.print("\t\tDescriptor ");
  Serial.print(descriptor.uuid());

  // read the descriptor value
  descriptor.read();

  // print out the value of the descriptor
  Serial.print(", value 0x");
  printData(descriptor.value(), descriptor.valueLength());

  Serial.println();
}
