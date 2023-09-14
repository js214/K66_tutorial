// Standard C files
#include <string.h>

// NXP Kinetis SDK files
#include "fsl_debug_console.h"
#include "fsl_i2c.h"

// board-specific files
#include "board/pin_mux.h"
#include "board/board.h"
#include "board/clock_config.h"

// project-specific files
#include "LMK05028_reg.h"

// I2C parameters
#define I2C_MASTER_CLK_SRC   I2C0_CLK_SRC
#define I2C_MASTER_CLK_FREQ  CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_MASTER_BASEADDR  I2C0
#define I2C_BAUDRATE         100000U
#define I2C_SLAVE_ADDR_7BIT  96U

// global variables
const bool print_debug = false;
volatile bool g_MasterCompletionFlag;
volatile uint32_t g_systickCounter;

// I2C callback function to signal transfer success.
static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
   if (status == kStatus_Success)
      g_MasterCompletionFlag = true;
}

// system tick timer handler function
void SysTick_Handler(void)
{
   if (g_systickCounter != 0U)
      g_systickCounter--;
}

// delay for N timer ticks
void SysTick_DelayTicks(uint32_t N)
{
   g_systickCounter = N;
   while (g_systickCounter != 0U);
}

/**
 * A function to write a single register of the LMK05028 device.
 *
 * @param reg_addr_h register address (high byte)
 * @param reg_addr_l register address (low byte)
 * @param data one byte of data to write to the register
 */
static uint8_t write_reg(const uint16_t reg_addr_h, const uint16_t reg_addr_l, const uint16_t data)
{
   // initialize I2C config
   i2c_master_config_t masterConfig;
   I2C_MasterGetDefaultConfig(&masterConfig);
   masterConfig.baudRate_Bps = I2C_BAUDRATE;
   I2C_MasterInit(I2C_MASTER_BASEADDR, &masterConfig, (uint32_t)I2C_MASTER_CLK_FREQ);

   // initialize I2C transfer handle
   i2c_master_handle_t g_m_handle;
   memset(&g_m_handle, 0, sizeof(g_m_handle));
   I2C_MasterTransferCreateHandle(I2C_MASTER_BASEADDR, &g_m_handle, i2c_master_callback, NULL);
   i2c_master_transfer_t masterXfer;

   // setup I2C transfer (write two bytes)
   uint8_t txBuff[] = {reg_addr_h, reg_addr_l, data};
   memset(&masterXfer, 0, sizeof(masterXfer));
   masterXfer.slaveAddress   = I2C_SLAVE_ADDR_7BIT;
   masterXfer.direction      = kI2C_Write;
   masterXfer.subaddress     = NULL;
   masterXfer.subaddressSize = 0;
   masterXfer.data           = txBuff;
   masterXfer.dataSize       = 3;
   masterXfer.flags          = kI2C_TransferDefaultFlag;

   // execute the I2C transfer
   g_MasterCompletionFlag = false;
   I2C_MasterTransferNonBlocking(I2C_MASTER_BASEADDR, &g_m_handle, &masterXfer);
   while (!g_MasterCompletionFlag);
   g_MasterCompletionFlag = false;
}

void setup_MCU(void)
{
   // initialize board
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
   BOARD_InitDebugConsole();

   // set systick reload value to generate 1ms interrupts
   if (SysTick_Config(SystemCoreClock / 1000U))
      while (1);
}

void setup_LMK05028(void)
{
   // set all registers
   const int N = 677;
   for (uint16_t i=0; i<N; i++) {
      // write to the LMK05028 device registers
      write_reg(LMK05028_address_high[i], LMK05028_address_low[i], LMK05028_data[i]);

      // print the data just written to UART debug console
      if (print_debug) {
         uint16_t addr = (uint16_t)((LMK05028_address_high[i])<<8) + LMK05028_address_low[i];
         uint8_t data = LMK05028_data[i];
         PRINTF("0x%4x : 0x%2x\r\n", addr, data);
      }
   }

   // toggle PLL1 off and on again
   write_reg(0x00, 0x5a, 0x03);
   SysTick_DelayTicks(500U);
   write_reg(0x00, 0x5a, 0x02);
}

int main(void)
{
   setup_MCU();
   setup_LMK05028();

   while (1) {
      char cmd = GETCHAR();
      switch (cmd) {
         // turn PLL1 off
         case '0' :
            write_reg(0x00, 0x5a, 0x03);
            break;

         // turn PLL1 on
         case '1' :
            write_reg(0x00, 0x5a, 0x02);
            break;

         // set PLL2 numerator (10 MHz out)
         case 't' :
            write_reg(0x00, 0x83, 0x49);
            write_reg(0x00, 0x84, 0x33);
            write_reg(0x00, 0x85, 0x3c);
            write_reg(0x00, 0x86, 0xcf);
            write_reg(0x00, 0x87, 0x9c);
            break;

         // detune PLL2 numerator
         case 'd' :
            write_reg(0x00, 0x83, 0x49);
            write_reg(0x00, 0x84, 0x33);
            write_reg(0x00, 0x85, 0x4c);
            write_reg(0x00, 0x86, 0xcf);
            write_reg(0x00, 0x87, 0x9c);
            break;
      }
   }
}
