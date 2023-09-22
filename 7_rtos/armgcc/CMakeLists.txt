# project name and directory
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
project(main.elf)
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})
SET(MCUX_SDK_PROJECT_NAME ${CMAKE_PROJECT_NAME})

# Misc cmake/compiler options
SET(CMAKE_SYSTEM_NAME Generic)
ENABLE_LANGUAGE(ASM)
include(${ProjDirPath}/flags.cmake)
SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)
SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})

# Source code to build the main executable target
add_executable(${MCUX_SDK_PROJECT_NAME} 
   "${ProjDirPath}/../main.c"
   "${ProjDirPath}/../board/board.c"
   "${ProjDirPath}/../board/board.h"
   "${ProjDirPath}/../board/pin_mux.c"
   "${ProjDirPath}/../board/pin_mux.h"
   "${ProjDirPath}/../board/clock_config.c"
   "${ProjDirPath}/../board/clock_config.h"
   )

# Where to find modules (.cmake files)
set(CMAKE_MODULE_PATH
   ${ProjDirPath}/../CMSIS
   ${ProjDirPath}/../components/uart
   ${ProjDirPath}/../device
   ${ProjDirPath}/../drivers
   ${ProjDirPath}/../freertos
   ${ProjDirPath}/../utilities
   )

# Modules (.cmake files) to include
include(CMSIS_Include_core_cm_MK66F18)
include(device_MK66F18_CMSIS_MK66F18)
include(device_MK66F18_startup_MK66F18)
include(device_MK66F18_system_MK66F18)
include(driver_clock_MK66F18)
include(driver_common_MK66F18)
include(driver_flash_MK66F18)
include(driver_gpio_MK66F18)
include(driver_lpuart_MK66F18)
include(driver_port_MK66F18)
include(driver_smc_MK66F18)
include(driver_uart_MK66F18)
include(freertos)
include(utilities_misc_utilities_MK66F18)
include(utility_assert_lite_MK66F18)
include(utility_debug_console_lite_MK66F18)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE freertos_kernel)
