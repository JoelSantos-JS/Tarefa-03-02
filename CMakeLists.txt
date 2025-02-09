cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)
project(TarefaSerial C CXX ASM)
pico_sdk_init()

add_executable(TarefaSerial 
    TarefaSerial.c 
    matriz_leds.c 
    incre/ssd1306.c
)

pico_set_program_name(TarefaSerial "TarefaSerial")
pico_set_program_version(TarefaSerial "0.1")

pico_generate_pio_header(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/pio_matrix.pio)

# Importante: USB habilitado (1) e UART desabilitado (0)
pico_enable_stdio_uart(TarefaSerial 0)
pico_enable_stdio_usb(TarefaSerial 1)

target_include_directories(TarefaSerial PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/incre
)

target_link_libraries(TarefaSerial 
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_clocks
)

pico_add_extra_outputs(TarefaSerial)