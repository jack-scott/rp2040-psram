set(MICROPY_MODULE_PSRAM_ENABLED 1)
add_library(usermod_psram INTERFACE)
target_sources(usermod_psram INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/mpy_psram_spi.c
    ${CMAKE_CURRENT_LIST_DIR}/psram_spi.c
)
target_include_directories(usermod_psram INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)
