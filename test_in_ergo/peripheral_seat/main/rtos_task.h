#pragma once
//#include <Arduino.h>
//#include <cstddef>
//#include <cstdint>
//#include "globals.h"
//#include <imu_header.h>
//
/// VAR///
//extern timer_0;


// Handlers
//TaskHandle_t LED_Task_Handle = NULL; 
TaskHandle_t Task_IMURead_Handle = NULL;
//TaskHandle_t Task_BLE_Handle = NULL;


//TaskHandle_t Task_LED_Handle 				= NULL; 
TaskHandle_t Task_FSRRead_Handle 		= NULL;
TaskHandle_t Task_SerialShow_Handle 	= NULL;
TaskHandle_t Task_BLE_Handle 				= NULL;
//TaskHandle_t Task_Monitor_Handle		 	= NULL;
//TaskHandle_t Task_QueueMonitor_Handle	= NULL;
TaskHandle_t Task_TimeKeeper_Handle	= NULL;
TaskHandle_t Task_LocalTimerWacher_Handle	= NULL;

// Queues
QueueHandle_t FSR_fifo; // fifo for the FSR data
QueueHandle_t IMU_fifo;
QueueHandle_t t0Queue; // Fifo for the change of time


// defines
#define FIFO_SIZE 3000
#define IMU_FIFO_SIZE 3000
#define T0_FIFO_SIZE 16

void TaskLEDTest(void *pvParameters);
void TaskFSRRead(void*pvParameters);
void TaskSerialShow(void *pvParameters);
void TaskBLE(void *pvParameters);
void TaskMonitor(void *pvParameters);
void TaskQueueMonitor(void *pvParameters);
void printFSRData(FSR_data_t &data);
static void TaskTimeKeeper(void* pvParameters);
void printFSRData_2(FSR_data_t &data);
void TaskLocalTimerWatcher(void *pvParameters);
void TaskReadIMUData(void *pvParameters);
///////////////////////ISR////////////////
///
// Interrupt routine that captures the current time and queues it.
static void IRAM_ATTR update_timer_from_ISR()
{	
	if(!t0Queue)return;
   BaseType_t hpw = pdFALSE;
   TickMsg m{esp_timer_get_time()};
   xQueueSendFromISR(t0Queue,&m,&hpw);
   if(hpw)portYIELD_FROM_ISR();
}


//}


////////////////////////////////Init Task///////////////////////////////////

// Create queues, attach interrupts and spawn all FreeRTOS tasks.
void init_freertos_tasks()
{ 
			/////////////////////////////////fifo decla///////
	IMU_fifo = xQueueCreate(IMU_FIFO_SIZE,sizeof(IMU_data_t));
	if(IMU_fifo==NULL)
	{Serial.print("error creating queue 1"); while(1);}
	
  	t0Queue = xQueueCreate(T0_FIFO_SIZE, sizeof(TickMsg));
	if(t0Queue==NULL)
	{Serial.print("error creating queue 2"); while(1);}
	////////////////////////

	//configASSERT(t0Queue);
	//configASSERT(IMU_fifo);

  	pinMode(PIN_IRQ, INPUT_PULLDOWN);// pin del interrupt
  	//attachInterrupt(digitalPinToInterrupt(PIN_IRQ), update_timer_from_ISR, RISING);
	//xTaskCreate(TaskTimeKeeper,"TaskTimeKeeper",4096,NULL,21,&Task_TimeKeeper_Handle);
	//xTaskCreate(TaskLocalTimerWatcher,"LocalTimerwatcher",4096,NULL,13,&Task_LocalTimerWacher_Handle);
	
	xTaskCreate(TaskReadIMUData,"TaskReadIMUData",6000,NULL,15,&Task_IMURead_Handle);
	//xTaskCreate(TaskLEDTest,"TaskLEDTest",4000,NULL,2,&Task_LED_Handle);
	//xTaskCreate(TaskFSRRead,"TaskFSRRead",8096,NULL,10,&Task_FSRRead_Handle);
	xTaskCreate(TaskBLE,"TaskBLE",8096,NULL,20,&Task_BLE_Handle);

                //task monitor
   //xTaskCreate(TaskMonitor, "TaskMonitor", 2548, NULL, 1, &Task_Monitor_Handle);
   //xTaskCreate(TaskQueueMonitor, "QueueMonitor", 2548, NULL, 1, &Task_QueueMonitor_Handle);


		//vTaskSuspendAll();
		//vTaskResume(&Task_TimeKeeper_Handle);

}



//////////////////////////////Task declaration/////////////////////////////
// Periodically print the current timer value for debugging.
void TaskLocalTimerWatcher(void *pvParameters)
{ 
	while (1)
	{
		Serial.print("timer_0= ");
		Serial.println(timer_0);
		vTaskDelay(pdMS_TO_TICKS(3000));
	}
}

// Updates timer_0 based on timestamps received from the ISR.
static void TaskTimeKeeper(void  *pvParameters)
{ 
	TickMsg m;
	while(1)
	{
		if(xQueueReceive(t0Queue,&m,portMAX_DELAY)==pdTRUE)
		{
			portENTER_CRITICAL(&t0mux);
			timer_0=m.stamp_us;
			portEXIT_CRITICAL(&t0mux);
			//Serial.println(timer_0);
		}
		//vTaskDelay(pdMS_TO_TICKS(1));
	}

}

void TaskLEDTest(void *pvParameters)
{
	while (true) 
	{
		for (int i = 0; i<255;i++) {LED(i,0,0);vTaskDelay(pdMS_TO_TICKS(10));}
		for (int i = 0; i<255;i++) {LED(0,i,0);vTaskDelay(pdMS_TO_TICKS(10));}
		for (int i = 0; i<255;i++) {LED(0,0,i);vTaskDelay(pdMS_TO_TICKS(10));}

	}
}



/// This task create an array for holding the data from the IMU 
/// and a var for the TimeStamp of this data. 
/// Then add the data to a FIFO for been used in other Task
/// data sampling = 125hz
void TaskReadIMUData(void *pvParameters)
{
	IMU_data_t dato;
	float IMU_snapshot[23];
	int64_t time_stamp_var;


	while(true)
	{
		IMU_read(IMU_snapshot, &time_stamp_var);
	
		//MUY OPTIMIZABLE REHACER PARAR PASAR POR REFERENCIA.
		dato.time_stamp=time_stamp_var;
		dato.acc_x = IMU_snapshot[8];
		dato.acc_y = IMU_snapshot[9];
		dato.acc_z = IMU_snapshot[10];
		dato.gyr_x = IMU_snapshot[11];
		dato.gyr_y = IMU_snapshot[12];
		dato.gyr_z = IMU_snapshot[13];
		dato.mag_x = IMU_snapshot[14];
		dato.mag_y = IMU_snapshot[15];
		dato.mag_z = IMU_snapshot[16];

		if(xQueueSend(IMU_fifo,&dato,portMAX_DELAY)==pdPASS)
		{//Serial.println("data added to FIFO ");
		 }
		else{Serial.println("FIFO FULL");}
		vTaskDelay(pdMS_TO_TICKS(IMU_MS_UPDATE));
	
	}
}


// Simple task that blinks the built-in LED once per second.
//void TaskLEDTest(void *pvParameters)
//{
//        while (true)
//        {
//                digitalWrite(LED_BUILTIN,1);
//                vTaskDelay(pdMS_TO_TICKS(1000));
//                digitalWrite(LED_BUILTIN,0);
//                vTaskDelay(pdMS_TO_TICKS(1000));
//        }
//}


// Read FSR sensors and push samples into the queue.
void TaskFSRRead(void *pvParameters)
{
        FSR_data_t data;
	//int64_t t_temp;
	//int64_t get_local_time_us()

	while (true) 
	{
		data.time_stamp =	get_local_time_us();
	
		data.FSR_0 =analogRead(A0); 
      data.FSR_1 =100;//analogRead(A1);
      data.FSR_2 =100;//analogRead(A2);
      data.FSR_3 =100;//analogRead(A3);
		data.FSR_4 =100;//analogRead(A4); 
		data.FSR_5 =100;//analogRead(A5); 
		data.FSR_6 =100;//analogRead(A6); 
		data.FSR_7 =100;//analogRead(A7); 



		// int fsr0 = (analogRead(A4) > 1800) ? 1 : 0;
  //int fsr1 = (analogRead(A5) > 1400) ? 1 : 0;
  //int fsr2 = (analogRead(A6) > 1200) ? 1 : 0;
  //int fsr3 = (analogRead(A7) > 1600) ? 1 : 0;

	
		if(xQueueSend(FSR_fifo,&data,portMAX_DELAY)==pdPASS)
		{//Serial.println("data added to FIFO ");
		 	printFSRData_2(data);
		}
		else{Serial.println("FIFO FULL");}	
		//vTaskDelay(pdMS_TO_TICKS(int(1000/FSR_SAMPLE_RATE)));
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
///////////////////// TaskBLE//////////////////////
// Handles BLE connections and transmits queued FSR data.
// Es recomendable que el stack ocupado por el BLE este inicializado antes de
// su uso, por ende, ojala iniciarlo en el setup o antes del bucle principal
// de la TASK
void TaskBLE(void *pvParameters)
{
   IMU_data_t d;
	char buffer[sizeof(IMU_data_t)+10];

	vTaskSuspend(Task_IMURead_Handle);
	while (true) 
	{
  	 	//BLEDevice central = BLE.central();
  		rainbow(); // rainbow led till the Bluetooth is connected

  		if(NimBLEDevice::getServer()->getConnectedCount())
  		{
				
  		  Serial.println("Connected to central device");
  		  Serial.print("Device MAC address: ");
  		  //Serial.println(central.address());	    
		  vTaskResume(Task_IMURead_Handle);
		  LED(1u,1u,1u);// device connected and sending data 
  			while (NimBLEDevice::getServer()->getConnectedCount()) // Mientras siga conectado
      	{
        		if (xQueueReceive(IMU_fifo, &d, portMAX_DELAY) == pdPASS)
        		{
          		imuDataToCSV_char(&d, buffer, sizeof(buffer));

          		// Enviar por BLE
          		pSensorCharacteristic->setValue((uint8_t*)buffer, strlen(buffer));
          		pSensorCharacteristic->notify(); // Notifica el valor actual
        		}
      	}
  		  LED(0,0,0);
		  // Re advertising if get disconected
		  Serial.print("Client disconected");
		  NimBLEDevice::startAdvertising();
  		
  		}

	}
}




//funtions/////////
//
// Debug print of first four FSR values.
void printFSRData(FSR_data_t &data)
{
        Serial.printf("t: %d, F0: %d, F1: %d, F2: %d, F3: %d \n",data.time_stamp,data.FSR_0,data.FSR_1,data.FSR_2,data.FSR_3);
}

// Print the entire FSR dataset as comma-separated values.
void printFSRData_2(FSR_data_t &data)
{
        Serial.print(data.time_stamp);
        Serial.print(",");
        Serial.print(data.FSR_0);
        Serial.print(",");
        Serial.print(data.FSR_1);
        Serial.print(",");
        Serial.print(data.FSR_2);
        Serial.print(",");
        Serial.print(data.FSR_3);
        Serial.print(",");
        Serial.print(data.FSR_4);
        Serial.print(",");
        Serial.print(data.FSR_5);
        Serial.print(",");
        Serial.print(data.FSR_6);
        Serial.print(",");
        Serial.println(data.FSR_7);

}



