// FreeRTOS files
#include "FreeRTOS.h"
#include "task.h"

// NXP Kinetis SDK files
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

// board-specific files
#include "board/pin_mux.h"
#include "board/clock_config.h"
#include "board/board.h"

/**
 * Calls functions to setup the microcontroller.
 */
void setup_MCU(void)
{
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
   BOARD_InitDebugConsole();
}


/**
 * Prints Hello world to the debug console.
 *
 * After printing the message, the task deletes itself.
 */
void vTask1(void *pvParameters)
{
   PRINTF("Hello world.\r\n");
   vTaskDelete(NULL);
}


/**
 * Toggles an LED on and off.
 *
 * After toggling the LED, the task pauses for a second.
 */
void vTask2(void *pvParameters)
{
   LED_BLUE_INIT(0);

   while (true) {
      LED_BLUE_TOGGLE();
      vTaskDelay(pdMS_TO_TICKS(1000));
   }

   vTaskDelete(NULL); // never reached
}


int main(void)
{
   setup_MCU();

   xTaskCreate(
         vTask1,   // function that implements the task
         "Task 1", // text name for the task
         1000,     // stack depth
         NULL,     // not using the task parameter
         1,        // priority
         NULL);    // not using a task handle

   xTaskCreate(vTask2, "Task 2", 1000, NULL, 1, NULL);

   vTaskStartScheduler();

   while (true); // never reached
}
