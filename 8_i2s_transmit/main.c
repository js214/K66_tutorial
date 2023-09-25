// NXP Kinetis SDK files
#include "fsl_debug_console.h"
#include "fsl_codec_common.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_codec_adapter.h"
#include "fsl_dialog7212.h"

// board-specific files
#include "board/pin_mux.h"
#include "board/clock_config.h"
#include "board/board.h"
#include "board/peripherals.h"

// project include files
#include "music.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_SAI_CLKSRC       kCLOCK_CoreSysClk
#define DEMO_SAI_CLK_FREQ     CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_SAI_IRQ          I2S0_Tx_IRQn
#define DEMO_SAITxIRQHandler  I2S0_Tx_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MCLK_OUTPUT  true
#define DEMO_SAI_MASTER_SLAVE kSAI_Master


#define DEMO_I2C              I2C1
#define DEMO_I2C_CLKSRC       kCLOCK_BusClk
#define DEMO_I2C_CLK_FREQ     CLOCK_GetFreq(kCLOCK_BusClk)
#define I2C_RELEASE_SDA_PORT  PORTC
#define I2C_RELEASE_SCL_PORT  PORTC
#define I2C_RELEASE_SDA_GPIO  GPIOC
#define I2C_RELEASE_SDA_PIN   11U
#define I2C_RELEASE_SCL_GPIO  GPIOC
#define I2C_RELEASE_SCL_PIN   10U
#define I2C_RELEASE_BUS_COUNT 100U

#define BOARD_MASTER_CLOCK_CONFIG        BOARD_MasterClockConfig
#define DEMO_CODEC_DA7212

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void BOARD_I2C_ReleaseBus(void);
void BOARD_MasterClockConfig(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
da7212_config_t da7212Config = {
   .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = 60000000},
   .dacSource    = kDA7212_DACSourceInputStream,
   .slaveAddress = DA7212_ADDRESS,
   .protocol     = kDA7212_BusI2S,
   .format       = {.mclk_HZ = I2S0_TX_BCLK_SOURCE_CLOCK_HZ, .sampleRate = 16000, .bitWidth = 16},
   .isMaster     = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_DA7212, .codecDevConfig = &da7212Config};

sai_master_clock_t mclkConfig;

sai_handle_t txHandle           = {0};
static volatile bool isFinished = false;
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_MasterClockConfig(void)
{
   mclkConfig.mclkOutputEnable = true, mclkConfig.mclkHz = I2S0_TX_BCLK_SOURCE_CLOCK_HZ;
   mclkConfig.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;
   SAI_SetMasterClockConfig(I2S0_PERIPHERAL, &mclkConfig);
}

static void callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
   isFinished = true;
}

void DelayMS(uint32_t ms)
{
   for (uint32_t i = 0; i < ms; i++)
   {
      SDK_DelayAtLeastUs(1000, SystemCoreClock);
   }
}

/*!
 * @brief Main function
 */
int main(void)
{
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
   BOARD_InitDebugConsole();
   BOARD_InitBootPeripherals();

   PRINTF("SAI example started!\n\r");

   /* SAI init */
   SAI_TransferTxCreateHandle(I2S0_PERIPHERAL, &txHandle, callback, NULL);

   /* I2S mode configurations */
   //SAI_TxSetConfig(I2S0_PERIPHERAL, &I2S0_Tx_config);
   SAI_TransferTxSetConfig(I2S0_PERIPHERAL, &txHandle, &I2S0_Tx_config);

   /* set bit clock divider */
   SAI_TxSetBitClockRate(I2S0_PERIPHERAL, I2S0_TX_BCLK_SOURCE_CLOCK_HZ, I2S0_TX_SAMPLE_RATE, I2S0_TX_WORD_WIDTH, 
         I2S0_TX_WORDS_PER_FRAME);

   if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
      assert(false);

   if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
            100U) != kStatus_Success)
      assert(false);

   /* delay for codec output stable */
   DelayMS(1000U);

   /*  xfer structure */
   uint32_t temp = (uint32_t)music;
   sai_transfer_t xfer;
   xfer.data     = (uint8_t *)temp;
   xfer.dataSize = MUSIC_LEN;
   SAI_TransferSendNonBlocking(I2S0_PERIPHERAL, &txHandle, &xfer);
   /* Wait until finished */
   while (isFinished != true);

   PRINTF("\n\r SAI example finis!\n\r ");
   while (true);
}
