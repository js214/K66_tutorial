# FreeRTOS internal cmake file. Do not use it in user top-level project

add_library(freertos_kernel_port STATIC
    $<$<STREQUAL:${FREERTOS_PORT},GCC_ARM_CM4F>:
        GCC/ARM_CM4F/port.c>
)

target_link_libraries(freertos_kernel_port
    PRIVATE
        freertos_kernel
)
