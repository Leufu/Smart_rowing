/*
 * La tarea int16_t ADC=readRaw(ADS1115_COMP_0_GND); toma 6190uS en proedio
 */

#include "ADS.h"
#include "utilidades.h"
#include "BLE.h"




void setup() 
{

  Begin_Task();
  
}

void loop() 
{
  BLEDevice central = BLE.central();
  
  int16_t adc0= readRaw(ADS1115_COMP_0_GND);
  //Serial.println("-----------------------------------------------------------");
  Serial.print("AIN0: "); Serial.println(adc0);

  ADS_A0_char.writeValue(adc0);
  //ADS_A0_char.writeValue(32740);
  /*
   * 
   
  if(central)
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    delay(1000);
    
    while (central.connected()) 
    {
      
      adc0 = ads.getRawResult();
      ADS_A0_char.writeValue(adc0); // write 0 value to the char
      Serial.print("ADC0:");Serial.print(float(adc0));
    }
    
    
    
  }
  */



  
  //Halt_Task();
  
}

/// begin//

void Begin_Task()
{
  
  Wire.begin();
  Serial.begin(115200);

  //////////////////////config ADS1115
  if (!ads.init()) {
    Serial.println("Failed to initialize ADc.");
    while (1);
  }
  
  ads.setVoltageRange_mV(ADS1115_RANGE_6144); //comment line/change parameter to change range
  ads.setCompareChannels(ADS1115_COMP_0_GND); //comment line/change parameter to change channel
  ads.setConvRate(ADS1115_860_SPS);
  ads.setMeasureMode(ADS1115_CONTINUOUS);
  ///////////////////////////////////////


  ////////////////// se inicia monitoreo///////////
  //while(!Serial);
  Serial.println("Start monitoring!");
  ///////////////////////////////////////////////
  ////////////////////////BLE/////////////////////////
  if (!BLE.begin()){Serial.println("BLE.begin() failed");}
  
  BLE.setLocalName("Sense PATAS"); ///Se le pone nombre al dispositivo
  BLE.setAdvertisedService(ADS_service); //Se crea un service inside of the peripheral
  ADS_service.addCharacteristic(ADS_A0_char); // Add characteristic to the peripheral
  ADS_A0_char.writeValue(0); // write 0 value to the char

  BLE.addService(ADS_service); // adds service to with its chars to the peripheral device
  

  BLE.advertise();
  //////////////////////////////////////////////
}







void promediando()
{
  float promedio_2=0;
  for(int i=0;i<500;i++)
  {
    Time_Start_Task();
    int16_t ADC=readRaw(ADS1115_COMP_0_GND);
    Time_Finish_Task("Tomar dato del ADS");
    promedio_2+=elapsedTime;
    
  }
  float resultado= promedio_2/500;
  Serial.print("el resultado del promedio en ms:"); Serial.print(resultado);
  Halt_Task();
  
}
