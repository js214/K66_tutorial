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


project(main)

set(MCUX_SDK_PROJECT_NAME main.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../music.h"
"${ProjDirPath}/../board/pin_mux.c"
"${ProjDirPath}/../board/pin_mux.h"
"${ProjDirPath}/../board/board.c"
"${ProjDirPath}/../board/board.h"
"${ProjDirPath}/../board/clock_config.c"
"${ProjDirPath}/../board/clock_config.h"
"${ProjDirPath}/../board/peripherals.c"
"${ProjDirPath}/../board/peripherals.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/board
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../drivers
    ${ProjDirPath}/../components/codec/da7212
    ${ProjDirPath}/../components/codec
    ${ProjDirPath}/../components/codec/i2c
    ${ProjDirPath}/../components/i2c
    ${ProjDirPath}/../utilities/debug_console_lite
    ${ProjDirPath}/../device
    ${ProjDirPath}/../components/uart
    ${ProjDirPath}/../components/lists
    ${ProjDirPath}/../CMSIS
    ${ProjDirPath}/../utilities
)

# include modules
include(driver_i2c_MK66F18)
include(driver_dialog7212_MK66F18)
include(driver_codec_MK66F18)
include(driver_common_MK66F18)
include(component_da7212_adapter_MK66F18)
include(component_codec_i2c_MK66F18)
include(component_i2c_adapter_MK66F18)
include(utility_debug_console_lite_MK66F18)
include(utility_assert_lite_MK66F18)
include(driver_port_MK66F18)
include(driver_gpio_MK66F18)
include(driver_clock_MK66F18)
include(driver_sai_MK66F18)
include(driver_flash_MK66F18)
include(device_MK66F18_CMSIS_MK66F18)
include(component_uart_adapter_MK66F18)
include(component_lists_MK66F18)
include(driver_uart_MK66F18)
include(device_MK66F18_startup_MK66F18)
include(driver_lpuart_MK66F18)
include(driver_smc_MK66F18)
include(CMSIS_Include_core_cm_MK66F18)
include(utilities_misc_utilities_MK66F18)
include(device_MK66F18_system_MK66F18)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)
TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)
