#include "globals.h"

// Application entry point. Initializes hardware, BLE and FreeRTOS tasks.
void setup()
{
		
      Serial.begin(115200);
		//while(!Serial);
		Serial.println("start");
    	led_init();
    	IMU_Init(IMU_config,10,0xFFFFFFFF);
    	
    	while(err==1)
    	{
    	  Serial.print("intento IMU");
    	  delay(100);
    	  err=0;
    	  IMU_Init(IMU_config,10,0xFFFFFFFF);
    	  rainbow_error();
    	  //while(1){LED(100,0,0);delay(100);}
    	}
    	
    	//Init_BLE();
    	//init_freertos_tasks();


        // Built-in LED used as a simple status indicator
        //pinMode(LED_BUILTIN, OUTPUT);

        Init_BLE();                // start Bluetooth service
        init_freertos_tasks();     // create FreeRTOS tasks
}

// The main loop is left empty; all work is handled by FreeRTOS tasks.
void loop()
{
}
