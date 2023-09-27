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

void setup_MCU(void)
{
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
   BOARD_InitDebugConsole();
   BOARD_InitBootPeripherals();
}

void vTask1(void *pvParameters)
{
   while (true) {
      // set up the transfer data
      const uint8_t transfer_size = 3;
      uint8_t masterTxData[] = {0x12, 0x13, 0x14};

      // Start master transfer, send data to slave
      dspi_transfer_t masterXfer;
      masterXfer.txData      = masterTxData;
      masterXfer.rxData      = NULL;
      masterXfer.dataSize    = transfer_size;
      masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
      DSPI_MasterTransferBlocking(SPI0_PERIPHERAL, &masterXfer);

      // loop delay
      vTaskDelay(pdMS_TO_TICKS(100));
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

   vTaskStartScheduler();

   while (true); // never reached
}
