include_guard(GLOBAL)
message("device_MK66F18_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MK66F18.S
)


include(device_MK66F18_system_MK66F18)

