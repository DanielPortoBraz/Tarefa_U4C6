# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
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
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

# Definições do projeto
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)

project(tarefa_U4C6 C CXX ASM)

pico_sdk_init()

# Criar diretório para arquivos gerados
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)

# Criar o executável primeiro!
add_executable(tarefa_U4C6
    tarefa_U4C6.c       # Código principal
    inc/ssd1306.c       # Driver do OLED SSD1306
)

# Agora sim, gerar o header do PIO (target já existe)
pico_generate_pio_header(tarefa_U4C6 ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# Habilitar saída via UART e desabilitar USB
pico_enable_stdio_uart(tarefa_U4C6 1)
pico_enable_stdio_usb(tarefa_U4C6 1)

# Vincular bibliotecas necessárias
target_link_libraries(tarefa_U4C6
    PRIVATE
    pico_stdlib
    hardware_pio  # Para controlar o PIO
    hardware_i2c  # Para comunicação com o SSD1306
    hardware_uart # Para comunicação serial
)

# Gerar arquivos binários extras (HEX, UF2)
pico_add_extra_outputs(tarefa_U4C6)
