# Projeto Raspberry Pi Pico W - Interfaces de Comunicação I2C e UART

Este projeto tem como objetivo proporcionar o aprendizado dos conceitos de comunicação serial I2C e UART no microcontrolador RP2040, além de demonstrar o uso de interrupções e debounce. Utiliza-se o SDK Pico C/C++ para o desenvolvimento e manipulação de periféricos como LEDs RGB, display OLED e botões.

## Funcionalidades Principais

- **Matriz de LEDs RGB 5x5**: Controla uma matriz de LEDs utilizando o protocolo WS2812, permitindo acender LEDs com diferentes cores.
- **Display OLED SSD1306**: Exibe informações sobre o estado dos LEDs e outros caracteres.
- **Botões**: Controla LEDs RGB com dois botões físicos, A e B, com debounce para evitar leituras falsas.
- **Comunicação UART**: Envia e recebe dados via UART para comunicação serial.
- **Interrupções**: Gerencia interrupções de botões e UART para uma comunicação eficiente.

## Requisitos

- **Hardware**: Raspberry Pi Pico W
- **Software**: SDK Raspberry Pi Pico C/C++
- **Dependências**: 
  - Biblioteca `ws2812.pio` para controlar a matriz de LEDs.
  - Biblioteca `ssd1306` para controle do display OLED.

## Conexões de Hardware

### Pinos Configurados

- **Matriz de LEDs WS2812 (5x5)**:
  - Pino GPIO 7 para controle de dados.
  - LEDs RGB configurados com intensidade inicial.

- **Display OLED SSD1306 (I2C)**:
  - Pino GPIO 14 para SDA.
  - Pino GPIO 15 para SCL.
  - Endereço I2C: `0x3C`.

- **Botões**:
  - Botão A no GPIO 5.
  - Botão B no GPIO 6.

- **LEDs RGB**:
  - LED vermelho no GPIO 13.
  - LED verde no GPIO 11.
  - LED azul no GPIO 12.

- **UART**:
  - Pino GPIO 0 (TX).
  - Pino GPIO 1 (RX).
  - Taxa de transmissão: 115200 bauds.

## Configuração do Ambiente de Desenvolvimento

1. **Instalar o SDK do Raspberry Pi Pico**:
   - Siga as instruções [oficiais](https://www.raspberrypi.org/documentation/pico/getting-started/) para configurar o ambiente de desenvolvimento.

2. **Configuração do Projeto**:
   - Crie um diretório para o projeto e adicione o código-fonte no arquivo `main.c`.
   - Configure o CMakeLists.txt para compilar o projeto.

3. **Compilação e Upload**:
   - Compile o código com o CMake.
   - Faça o upload do arquivo compilado para o Raspberry Pi Pico W.

## Funcionalidades Detalhadas

### 1. **Controle da Matriz de LEDs WS2812**:
A matriz de LEDs RGB 5x5 é controlada utilizando o protocolo WS2812. Cada LED pode ser aceso em uma das cores (R, G, B), com intensidades configuráveis.

- Funções principais:
  - `set_one_led()`: Configura a cor dos LEDs com base no estado da matriz.
  - `put_pixel()`: Envia os dados de cor para os LEDs.
  
### 2. **Controle do Display OLED SSD1306**:
O display OLED é utilizado para exibir o estado dos LEDs e outros caracteres. O display é controlado via protocolo I2C.

- Funções principais:
  - `initialize_i2c()`: Inicializa o barramento I2C.
  - `ssd1306_init()`: Inicializa o display SSD1306.
  - `ssd1306_draw_string()`: Desenha strings no display.

### 3. **Botões e Interrupções**:
Os botões A e B são utilizados para controlar o estado dos LEDs RGB. Cada botão realiza uma ação de incremento ou decremento no LED correspondente. As interrupções de botões são tratadas com debounce para evitar múltiplas leituras de um único pressionamento.

- Funções principais:
  - `init_buttons()`: Configura os pinos GPIO para os botões.
  - `gpio_irq_handler()`: Função de interrupção que é chamada quando os botões são pressionados.

### 4. **Comunicação UART**:
A comunicação UART é utilizada para receber e enviar dados. O caractere recebido via UART é exibido na tela e também é refletido de volta (eco).

- Funções principais:
  - `initialize_uart()`: Inicializa a UART com a taxa de transmissão configurada.
  - `on_uart_rx()`: Função callback para ler dados recebidos pela UART.

### 5. **Debounce**:
A técnica de debounce é utilizada para garantir que a leitura dos botões não seja repetida rapidamente em sucessivas interrupções. Isso é feito adicionando um atraso de 200 ms após cada pressionamento de botão.

## Estrutura do Código

```c
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "ws2812.pio" 
#include "hardware/irq.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "numeros.h" // Biblioteca criada para os padrões numéricos de 0 a 9 no modelo da matriz 5x5
```

## Funcionalidades Principais

- **Matriz de LEDs RGB 5x5**: Controla uma matriz de LEDs utilizando o protocolo WS2812, permitindo acender LEDs com diferentes cores.
- **Display OLED SSD1306**: Exibe informações sobre o estado dos LEDs e outros caracteres.
- **Botões**: Controla LEDs RGB com dois botões físicos, A e B, com debounce para evitar leituras falsas.
- **Comunicação UART**: Envia e recebe dados via UART para comunicação serial.
- **Interrupções**: Gerencia interrupções de botões e UART para uma comunicação eficiente.

## Requisitos

- **Hardware**: Raspberry Pi Pico W
- **Software**: SDK Raspberry Pi Pico C/C++
- **Dependências**: 
  - Biblioteca `ws2812.pio` para controlar a matriz de LEDs.
  - Biblioteca `ssd1306` para controle do display OLED.

## Conexões de Hardware

### Pinos Configurados

- **Matriz de LEDs WS2812 (5x5)**:
  - Pino GPIO 7 para controle de dados.
  - LEDs RGB configurados com intensidade inicial.

- **Display OLED SSD1306 (I2C)**:
  - Pino GPIO 14 para SDA.
  - Pino GPIO 15 para SCL.
  - Endereço I2C: `0x3C`.

- **Botões**:
  - Botão A no GPIO 5.
  - Botão B no GPIO 6.

- **LEDs RGB**:
  - LED vermelho no GPIO 13.
  - LED verde no GPIO 11.
  - LED azul no GPIO 12.

- **UART**:
  - Pino GPIO 0 (TX).
  - Pino GPIO 1 (RX).
  - Taxa de transmissão: 115200 bauds.

## Funções Principais

### 1. **initialize_uart()**: 
Inicializa a comunicação UART. Configura os pinos de TX (GPIO 0) e RX (GPIO 1) e define a taxa de transmissão como 115200 bauds. Esta função permite a comunicação serial entre o Raspberry Pi Pico W e um dispositivo externo via UART.

### 2. **initialize_i2c()**:
Inicializa o barramento I2C para controle do display OLED SSD1306. Define os pinos GPIO 14 e 15 como SDA e SCL, respectivamente, e configura a velocidade de comunicação do I2C.

### 3. **init_buttons()**:
Configura os pinos GPIO 5 e 6 para os botões A e B. Além disso, ativa as interrupções para detectar o pressionamento dos botões e executa a técnica de debounce para evitar múltiplas leituras do mesmo evento.

### 4. **set_one_led()**:
Controla a cor de um LED específico na matriz de LEDs WS2812. Esta função é responsável por acender o LED com a cor desejada, utilizando o protocolo WS2812.

### 5. **gpio_irq_handler()**:
Função de interrupção que é chamada sempre que um dos botões A ou B é pressionado. A função aplica debounce para garantir que a leitura do botão seja feita de forma estável e que não haja múltiplas leituras do mesmo evento.

### 6. **ssd1306_init()**:
Inicializa o display OLED SSD1306 e o configura para exibir informações. Esta função é usada para garantir que o display esteja pronto para exibir os dados enviados posteriormente.

### 7. **ssd1306_draw_string()**:
Exibe uma string no display OLED SSD1306. Essa função permite atualizar a tela com texto informativo, como o status dos LEDs ou caracteres recebidos via UART.

### 8. **on_uart_rx()**:
Função callback chamada sempre que dados são recebidos pela UART. Os dados são exibidos no display OLED e também enviados de volta para o dispositivo, realizando um eco dos caracteres recebidos.

## Executando o Projeto

1. **Compilar o código**: Use o CMake para compilar o projeto.
2. **Carregar o programa** no Raspberry Pi Pico W.
3. **Conectar o Raspberry Pi Pico W** via USB para monitoramento serial.

A partir de agora, você pode interagir com os LEDs RGB através dos botões e visualizar os caracteres recebidos via UART no display OLED.

## Vídeo de demonstração
Link do vídeo no youtube:

