// Standard C files
#include <string.h>

// NXP Kinetis SDK files
#include "fsl_debug_console.h"
#include "fsl_i2c.h"

// board-specific files
#include "board/pin_mux.h"
#include "board/board.h"

// I2C parameters
#define I2C_MASTER_CLK_SRC   I2C0_CLK_SRC
#define I2C_MASTER_CLK_FREQ  CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_MASTER_BASEADDR  I2C0
#define I2C_BAUDRATE         100000U
#define I2C_SLAVE_ADDR_7BIT  96U

// I2C variables
volatile bool g_MasterCompletionFlag = false;

// I2C callback function to signal transfer success.
static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
   if (status == kStatus_Success)
      g_MasterCompletionFlag = true;
}

/**
 * A function to read a single register of the LMK05028 device.
 *
 * @param reg_addr_h register address (high byte)
 * @param reg_addr_l register address (low byte)
 *
 * @return register data byte
 */
static uint8_t read_reg(const uint16_t reg_addr_h, const uint16_t reg_addr_l)
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
   uint8_t txBuff[] = {reg_addr_h, reg_addr_l};
   memset(&masterXfer, 0, sizeof(masterXfer));
   masterXfer.slaveAddress   = I2C_SLAVE_ADDR_7BIT;
   masterXfer.direction      = kI2C_Write;
   masterXfer.subaddress     = NULL;
   masterXfer.subaddressSize = 0;
   masterXfer.data           = txBuff;
   masterXfer.dataSize       = 2;
   masterXfer.flags          = kI2C_TransferDefaultFlag;

   // execute the I2C transfer
   I2C_MasterTransferNonBlocking(I2C_MASTER_BASEADDR, &g_m_handle, &masterXfer);
   while (!g_MasterCompletionFlag);
   g_MasterCompletionFlag = false;

   // setup I2C transfer (read one byte)
   uint8_t rxBuff[] = {0};
   memset(&masterXfer, 0, sizeof(masterXfer));
   masterXfer.slaveAddress   = I2C_SLAVE_ADDR_7BIT;
   masterXfer.direction      = kI2C_Read;
   masterXfer.subaddress     = NULL;
   masterXfer.subaddressSize = 0;
   masterXfer.data           = rxBuff;
   masterXfer.dataSize       = 1;
   masterXfer.flags          = kI2C_TransferDefaultFlag;

   // execute the I2C transfer
   I2C_MasterTransferNonBlocking(I2C_MASTER_BASEADDR, &g_m_handle, &masterXfer);
   while (!g_MasterCompletionFlag);
   g_MasterCompletionFlag = false;

   return rxBuff[0];
}

int main(void)
{
   // initialize board
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
   BOARD_InitDebugConsole();

   // read N register values and print them over UART
   const int N = 0x311;
   for (uint16_t i=0; i<N; i++) {
      uint8_t data = read_reg((uint8_t)(i>>8), (uint8_t)i);
      PRINTF("0x%4x : ", i);
      PRINTF("0x%2x\r\n", data);
   }

   while (1);
}
