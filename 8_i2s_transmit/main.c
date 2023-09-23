// NXP Kinetis SDK files
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

// board-specific files
#include "board/pin_mux.h"
#include "board/clock_config.h"
#include "board/board.h"
#include "board/peripherals.h"

// global variables
sai_master_clock_t mclkConfig;
sai_handle_t txHandle = {0};
static volatile bool isFinished = false;

/**
 * Calls functions to setup the microcontroller.
 */
void setup_MCU(void)
{
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
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

   /* I2S mode configurations */
   SAI_GetClassicI2SConfig(&I2S0_Tx_config, I2S0_TX_WORD_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
   I2S0_Tx_config.syncMode    = kSAI_ModeAsync;
   I2S0_Tx_config.masterSlave = kSAI_Master;
   SAI_TransferTxSetConfig(I2S0_PERIPHERAL, &txHandle, &I2S0_Tx_config);

   /* set bit clock divider */
   SAI_TxSetBitClockRate(I2S0_PERIPHERAL, I2S0_TX_BCLK_SOURCE_CLOCK_HZ,
         I2S0_TX_SAMPLE_RATE, I2S0_TX_WORD_WIDTH, kSAI_Channel0Mask);

   /*  xfer structure */
   sai_transfer_t xfer;
   xfer.data     = (uint8_t *)music;
   xfer.dataSize = MUSIC_LEN;
   SAI_TransferSendNonBlocking(I2S0_PERIPHERAL, &txHandle, &xfer);

   /* Wait until finished */
   while (isFinished != true);

   while (true); // never reached
}
