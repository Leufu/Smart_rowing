//////////File para configurar ADS1115 

//////////////////////////////////ADS1115 config/////////////////
#include<ADS1115_WE.h> 
#include<Wire.h>
#define I2C_ADDRESS 0x48
ADS1115_WE ads = ADS1115_WE(I2C_ADDRESS);
//////////////////////////////////////////



float readChannel(ADS1115_MUX channel) {
  float voltage = 0.0;
  ads.setCompareChannels(channel);
  voltage = ads.getResult_V(); // alternative: getResult_mV for Millivolt
  return voltage;
}

int16_t readRaw(ADS1115_MUX channel)
{
  int16_t adc_raw =0;
  ads.setCompareChannels(channel);
  adc_raw = ads.getRawResult(); // alternative: getResult_mV for Millivolt
  return adc_raw;
}

float read_mV(ADS1115_MUX channel) {
  float voltage_mV = 0.0;
  ads.setCompareChannels(channel);
  voltage_mV = ads.getResult_mV(); // alternative: getResult_mV for Millivolt
  return voltage_mV;
}
