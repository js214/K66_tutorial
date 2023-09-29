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

// LMX2572 register data
#include "LMX2572_reg.h"

// bit mask for controlling the FPGA
#define ECHO_I2S_TO_SPI 0x01
#define I2S_CHANNEL 0x02
#define PRBS_ENABLE 0x04
#define BLANK_LEVEL 0x08
#define CNT_SEL0 0x10
#define CNT_SEL1 0x20
#define CNT_SEL2 0x40
#define CNT_SEL3 0x80
#define CNT_SEL4 0x100

// state variables
uint16_t conf_reg = 0x0000;

void setup_MCU(void)
{
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
   BOARD_InitDebugConsole();
   BOARD_InitBootPeripherals();
}


void setup_LMX2572(void)
{
   for (int i=0; i<126; i++)
      transmit_SPI(LMX2572_addr[i], LMX2572_data_h[i], LMX2572_data_l[i]);
}

/**
 * Handle keyboard (UART) commands
 */
void vTask1(void *pvParameters)
{
   while (true) {
      char cmd = GETCHAR();
      switch (cmd) {
         case '?' : // identify the MPU board
            PRINTF("LMX programmer code by JK.\r\n");
            break;

         case 'R' : // repeat setup of the LMX2572
            setup_LMX2572();
            break;

         case '0' : // clear FPGA configuration register
            conf_reg = 0x0000;
            transmit_SPI(0x80, conf_reg>>8, conf_reg);
            break;

         case 'p' : // toggle PRBS on/off
            conf_reg ^= PRBS_ENABLE;
            transmit_SPI(0x80, conf_reg>>8, conf_reg);
            break;

         case 's' : // toggle div-by-2 counter ("slow mode")
            conf_reg ^= CNT_SEL0;
            transmit_SPI(0x80, conf_reg>>8, conf_reg);
            break;

         case '1' : // toggle decimal counter (bit 1)
            conf_reg ^= CNT_SEL1;
            transmit_SPI(0x80, conf_reg>>8, conf_reg);
            break;

         case '2' : // toggle decimal counter (bit 2)
            conf_reg ^= CNT_SEL2;
            transmit_SPI(0x80, conf_reg>>8, conf_reg);
            break;

         case '3' : // toggle decimal counter (bit 3)
            conf_reg ^= CNT_SEL3;
            transmit_SPI(0x80, conf_reg>>8, conf_reg);
            break;

         case '4' : // toggle decimal counter (bit 4)
            conf_reg ^= CNT_SEL4;
            transmit_SPI(0x80, conf_reg>>8, conf_reg);
            break;

         case 'F' : // enable FSK SPI FAST
            transmit_SPI(0x72, 0x7C, 0x03);             // LMX: FSK_MODE_SEL=3, FSK_EN=1
            transmit_SPI(0x7C, 0x00, 0x01);             // LMX: FSK_SPI_FAST_DEV
            break;

         case 'f' : // set FSK deviation to zero
            transmit_SPI(0x7C, 0x00, 0x00);             // LMX: FSK_SPI_FAST_DEV
            break;

         case 'i' : // toggle echoing I2S to SPI
            conf_reg ^= ECHO_I2S_TO_SPI;
            transmit_SPI(0x80, conf_reg>>8, conf_reg);
            break;

         case 'c' : // toggle I2S channel (left or right)
            conf_reg ^= I2S_CHANNEL;
            transmit_SPI(0x80, conf_reg>>8, conf_reg);
            break;
      }
   }

   vTaskDelete(NULL); // never reached
}

int main(void)
{
   setup_MCU();
   setup_LMX2572();

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
