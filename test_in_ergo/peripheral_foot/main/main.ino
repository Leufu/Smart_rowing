#include "globals.h"

// Application entry point. Initializes hardware, BLE and FreeRTOS tasks.
void setup()
{
        Serial.begin(115200);

        // Built-in LED used as a simple status indicator
        pinMode(LED_BUILTIN, OUTPUT);

        Init_BLE();                // start Bluetooth service
        init_freertos_tasks();     // create FreeRTOS tasks
}

// The main loop is left empty; all work is handled by FreeRTOS tasks.
void loop()
{
}
