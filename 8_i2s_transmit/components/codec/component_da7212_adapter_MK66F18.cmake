include_guard(GLOBAL)
message("component_da7212_adapter component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/da7212/fsl_codec_da7212_adapter.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/da7212
    ${CMAKE_CURRENT_LIST_DIR}/port
)


include(driver_dialog7212_MK66F18)

include(driver_codec_MK66F18)

