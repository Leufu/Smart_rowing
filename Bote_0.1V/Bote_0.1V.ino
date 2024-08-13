#include "BLE.h"

#define per_name "Sense PATAS"
#define per_addr "32:22:2E:C8:BC:E5"
//#define SERVICE_UUID "0340"
//#define CHAR_UUID_ADS "2BF3"

//const char SERVICE_ONE_UUID= "0340";
//const char  CHAR_ONE_UUID  = "2BF3";

void setup() 
{
  Setup_Task();

}

void loop() 
{
  //Encontrar_PATAS_Task();
  BLEDevice peripheral = BLE.available();
  if (peripheral) 
  { if (peripheral.localName() == per_name)
    {
      
      BLE.stopScan();
          // connect to the peripheral
      if (peripheral.connect()) { Serial.println("Connected"); }
      else { Serial.println("Failed to connect!");return; }
    
      // discover peripheral attributes
      Serial.println("Discovering attributes ...");
      if (peripheral.discoverAttributes()) { Serial.println("Attributes discovered");} 
      else {Serial.println("Attribute discovery failed!"); peripheral.disconnect(); return; }
    
      // se crea la lista de servicios del periferico
      BLEService service = peripheral.service("0340");
      BLECharacteristic characteristic = service.characteristic("2BF3");
      BLECharacteristic characteristic_A1 = service.characteristic("2BF4");
      
      
      // read the characteristic value
      while(1)
      {
        if (characteristic.canRead()) 
        { characteristic.read();
          if (characteristic.valueLength() > 0) // print out the value of the characteristic
          {/*Serial.print(", value 0x")*/;
            //Serial.print("A0:");
            printData_le_DEC(characteristic.value(), characteristic.valueLength());Serial.print(",");
          }
        }
        //Serial.println();
        if (characteristic_A1.canRead()) 
        { characteristic_A1.read();
          if (characteristic_A1.valueLength() > 0) // print out the value of the characteristic
          {/*Serial.print(", value 0x")*/;
            //Serial.print("A1:");
            printData_le_DEC(characteristic_A1.value(), characteristic_A1.valueLength());Serial.println();
          }
        }
      }
      
      //while(1){Read_Char_Task(peripheral,"0340","2BF3");}
    }  
  }
}

void Setup_Task()
{
  Serial.begin(115200);
  while (!Serial); // no inicia hasta que se habre el servidor 
  if (!BLE.begin()){Serial.println("BLE.begin() failed");}
  BLE.scan();
   
}
/*

void Read_Char_Task(BLEDevice peripheral,const char &Service_UUID,const char &Char_UUID)
{
  // connect to the peripheral
  if (peripheral.connect()) { Serial.println("Connected"); }
  else { Serial.println("Failed to connect!");return; }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) { Serial.println("Attributes discovered");} 
  else {Serial.println("Attribute discovery failed!"); peripheral.disconnect(); return; }

  // se crea la lista de servicios del periferico
  BLEService service = peripheral.service(Service_UUID);
  BLECharacteristic characteristic = service.characteristic(Char_UUID);
  
  // read the characteristic value
  if (characteristic.canRead()) 
  { characteristic.read();
    if (characteristic.valueLength() > 0) // print out the value of the characteristic
    {Serial.print(", value 0x");printData(characteristic.value(), characteristic.valueLength());
    }
  }
  Serial.println();
  
  
  

  
  
}
*/

void Encontrar_PATAS_Task()
{
    // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // see if peripheral is a LED
    if (peripheral.localName() == per_name) {
      // stop scanning
      BLE.stopScan();

      explorerPeripheral(peripheral);

      // peripheral disconnected, we are done
      
      //while (1) {
      //  // do nothing
      //}
    }
  }  
}
