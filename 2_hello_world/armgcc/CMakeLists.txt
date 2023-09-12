# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(hello_world)

set(MCUX_SDK_PROJECT_NAME hello_world.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
   "${ProjDirPath}/../hello_world.c"
   "${ProjDirPath}/../board/board.c"
   "${ProjDirPath}/../board/board.h"
   "${ProjDirPath}/../board/pin_mux.c"
   "${ProjDirPath}/../board/pin_mux.h"
   "${ProjDirPath}/../board/clock_config.c"
   "${ProjDirPath}/../board/clock_config.h"
   )

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
   ${ProjDirPath}/..
   )

set(SDK_DIR "C:/Users/Jkastelic/Documents/prog/SDK_2_11_0_FRDM-K66F")

set(CMAKE_MODULE_PATH
   ${SDK_DIR}/devices/MK66F18/utilities/debug_console_lite
   ${SDK_DIR}/devices/MK66F18/drivers
   ${SDK_DIR}/devices/MK66F18
   ${SDK_DIR}/components/uart
   ${SDK_DIR}/components/lists
   ${SDK_DIR}/CMSIS/Core/Include
   ${SDK_DIR}/devices/MK66F18/utilities
   )

# include modules
include(utility_debug_console_lite_MK66F18)
include(utility_assert_lite_MK66F18)
include(driver_gpio_MK66F18)
include(driver_clock_MK66F18)
include(driver_flash_MK66F18)
include(driver_common_MK66F18)
include(device_MK66F18_CMSIS_MK66F18)
include(component_uart_adapter_MK66F18)
include(component_lists_MK66F18)
include(driver_uart_MK66F18)
include(device_MK66F18_startup_MK66F18)
include(driver_lpuart_MK66F18)
include(driver_port_MK66F18)
include(driver_smc_MK66F18)
include(CMSIS_Include_core_cm_MK66F18)
include(utilities_misc_utilities_MK66F18)
include(device_MK66F18_system_MK66F18)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)
