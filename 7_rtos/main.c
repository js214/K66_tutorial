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

void setup_MCU(void)
{
   BOARD_InitBootPins();
   BOARD_InitBootClocks();
   BOARD_InitDebugConsole();
}

int main(void)
{
   setup_MCU();

   PRINTF("Hello world.\r\n");

   while (1)
   {
      char ch = GETCHAR();
      PUTCHAR(ch);
   }
}
