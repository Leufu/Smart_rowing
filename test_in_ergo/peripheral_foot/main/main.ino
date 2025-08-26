#include "globals.h"
//extern PIN_IRQ;
void setup()
{
	
	Serial.begin(115200);
	
  	pinMode(LED_BUILTIN, OUTPUT);   // <--- importante para digitalWrite
	Init_BLE();
	init_freertos_tasks();

}
void loop()
{
	
}
