// NXP Kinetis SDK files
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

// board-specific files
#include "board/pin_mux.h"
#include "board/clock_config.h"
#include "board/board.h"
#include "board/peripherals.h"

// global variables
sai_handle_t txHandle = {0};
static volatile bool isFinished = false;

/**
 * Calls functions to setup the microcontroller.
 */
void setup_MCU(void)
{
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
   //BOARD_InitBootPeripherals();
   BOARD_InitDebugConsole();
}


static void callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
   isFinished = true;
}


int main(void)
{
   setup_MCU();

   /* SAI init */
   SAI_Init(I2S0_PERIPHERAL);
   SAI_TransferTxCreateHandle(I2S0_PERIPHERAL, &txHandle, callback, NULL);

//   /* I2S mode configurations */
//   SAI_TransferTxSetConfig(I2S0_PERIPHERAL, &txHandle, &I2S0_Tx_config);

   /*  xfer structure */
   sai_transfer_t xfer;
   uint8_t data[] = {1, 2, 3, 4, 5};
   xfer.data     = data;
   xfer.dataSize = 6;
   SAI_TransferSendNonBlocking(I2S0_PERIPHERAL, &txHandle, &xfer);
   PRINTF("test ");

//   /* Wait until finished */
//   while (isFinished != true);

   while (true); // never reached
}
