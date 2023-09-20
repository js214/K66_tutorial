include_guard(GLOBAL)
message("freertos component is included.")

cmake_minimum_required(VERSION 3.15)

# FreeRTOS port option
set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "FreeRTOS port name")

# Heap number or absolute path to custom heap implementation provided by user
set(FREERTOS_HEAP "4" CACHE STRING "FreeRTOS heap model number. 1 .. 5. Or absolute path to custom heap source file")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/portable ../armgcc/freertos)

add_library(freertos_kernel STATIC
    ${CMAKE_CURRENT_LIST_DIR}/croutine.c
    ${CMAKE_CURRENT_LIST_DIR}/event_groups.c
    ${CMAKE_CURRENT_LIST_DIR}/list.c
    ${CMAKE_CURRENT_LIST_DIR}/queue.c
    ${CMAKE_CURRENT_LIST_DIR}/stream_buffer.c
    ${CMAKE_CURRENT_LIST_DIR}/tasks.c
    ${CMAKE_CURRENT_LIST_DIR}/timers.c

    # If FREERTOS_HEAP is digit between 1 .. 5 - it is heap number, otherwise - it is path to custom heap source file
    $<IF:$<BOOL:$<FILTER:${FREERTOS_HEAP},EXCLUDE,^[1-5]$>>,${FREERTOS_HEAP},${CMAKE_CURRENT_LIST_DIR}/portable/MemMang/heap_${FREERTOS_HEAP}.c>
)

target_include_directories(freertos_kernel PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/portable
        ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM4F
)

target_link_libraries(freertos_kernel freertos_kernel_port)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/portable
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM4F
)
