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

// bit mask for controlling the FPGA
#define ECHO_I2S_TO_SPI 0x01
#define I2S_CHANNEL 0x02
#define PRBS_ENABLE 0x04
#define BLANK_LEVEL 0x08
#define COUNTER_WIDTH0 0x10
#define COUNTER_WIDTH1 0x20
#define COUNTER_WIDTH2 0x40
#define COUNTER_WIDTH3 0x80

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
      transmit_SPI(0x80, 0x00, 0x00);
      vTaskDelay(pdMS_TO_TICKS(1000));

      transmit_SPI(0x80, 0x00, COUNTER_WIDTH0);
      vTaskDelay(pdMS_TO_TICKS(1000));

      transmit_SPI(0x80, 0x00, PRBS_ENABLE);
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

   vTaskStartScheduler();

   while (true); // never reached
}


void transmit_SPI(const uint8_t addr, const uint8_t data_h, const uint8_t data_l)
{
   // set up the transfer data
   uint8_t masterTxData[] = {addr, data_h, data_l};

   // start master transfer, send data to slave
   dspi_transfer_t masterXfer;
   masterXfer.txData      = masterTxData;
   masterXfer.rxData      = NULL;
   masterXfer.dataSize    = 3;
   masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
   DSPI_MasterTransferBlocking(SPI0_PERIPHERAL, &masterXfer);
}
