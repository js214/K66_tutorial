// FreeRTOS files
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

// NXP Kinetis SDK files
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

// board-specific files
#include "board/pin_mux.h"
#include "board/clock_config.h"
#include "board/board.h"
#include "board/peripherals.h"

/**
 * Calls functions to setup the microcontroller.
 */
void setup_MCU(void)
{
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
   BOARD_InitDebugConsole();

   LED_RED_INIT(0);
   LED_BLUE_INIT(0);
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
 * After toggling the LED, the task pauses for a second using vTaskDelay().
 */
void vTask2(void *pvParameters)
{
   while (true) {
      LED_BLUE_TOGGLE();
      vTaskDelay(pdMS_TO_TICKS(1000));
   }

   vTaskDelete(NULL); // never reached
}


/**
 * Toggles an LED on and off.
 *
 * This is not a task, but rather a software timer callback function, called by
 * the auto-reload Timer 1.
 */
static void prvTimerCallback1(TimerHandle_t xTimer)
{
   LED_RED_TOGGLE();
}


int main(void)
{
   setup_MCU();

   // print Hello world to the UART debug console
   xTaskCreate(
         vTask1,   // function that implements the task
         "Task 1", // text name for the task
         1000,     // stack depth
         NULL,     // not using the task parameter
         1,        // priority
         NULL);    // not using a task handle

   // blink the blue LED and sleep via vTaskDelay()
   xTaskCreate(vTask2, "Task 2", 1000, NULL, 1, NULL);

   // blink the red RED using an auto-reload software timer
   TimerHandle_t xAutoReloadTimer = xTimerCreate(
         "Timer 1", pdMS_TO_TICKS(500), true, 0, prvTimerCallback1);
   xTimerStart(xAutoReloadTimer, 0);

   vTaskStartScheduler();

   while (true); // never reached
}
