# Projeto Raspberry Pi Pico W - Interfaces de Comunicação I2C e UART

Este projeto tem como objetivo proporcionar o aprendizado dos conceitos de comunicação serial I2C e UART no microcontrolador RP2040, além de demonstrar o uso de interrupções e debounce. Utiliza-se o SDK Pico C/C++ para o desenvolvimento e manipulação de periféricos como LEDs RGB, display OLED e botões. Para uso dos periféricos e aplicação prática, utiliza-se a placa BitDogLab e um cabo USB para comunicação serial. Contudo, o projeto pode ser simulado através da extensão Wokwi, para isso, veja os vídeos de demonstração. 

## Funcionalidades Principais

- **Matriz de LEDs RGB 5x5**: Controla uma matriz de LEDs para exibir o número recebido pela comunicação serial.
- **Display OLED SSD1306**: Exibe informações sobre o estado dos LEDs e o caractere de entrada.
- **Botões**: Controla LEDs RGB com dois botões físicos, A e B, com debounce para evitar leituras falsas.
- **Comunicação UART**: Envia e recebe dados via UART-USB para comunicação serial.
- **Interrupções**: Gerencia interrupções de botões para não sobrecarregar o uso da CPU do microcontrolador.

## Requisitos

- **Hardware**: Raspberry Pi Pico W
- **Software**:
  - SDK Raspberry Pi Pico C/C++.
  - Extensão Wokwi no VS Code.
    
- **Dependências**: 
  - Biblioteca `ws2812.pio` para controlar a matriz de LEDs.
  - Biblioteca `ssd1306.h` para controle do display OLED.

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

## Configuração do Ambiente de Desenvolvimento

1. **Instalar o SDK do Raspberry Pi Pico**:
   - Siga as instruções [oficiais](https://www.raspberrypi.org/documentation/pico/getting-started/) para configurar o ambiente de desenvolvimento.

2. **Configuração do Projeto**:
   - Crie um diretório para o projeto e adicione o código-fonte.
   - Configure o CMakeLists.txt para compilar o projeto.

3. **Compilação e Upload**:
   - Compile o código com o CMake.
   - Faça o upload do arquivo compilado para o Raspberry Pi Pico W.

## Funcionalidades Detalhadas

### 1. **Controle da Matriz de LEDs WS2812**:
A matriz de LEDs endereçáveis (WS2812) RGB 5x5 é controlada utilizando funções provenientes de um arquivo PIO (escrito em ASM). Cada LED pode ser aceso em uma das cores (R, G, B), com intensidades configuráveis.

- Funções principais:
  - `set_one_led()`: Ativa as cores para serem enviadas para cada LED. Isto é feito levando em conta o padrão de zigue-zague da matriz.
  - `put_pixel()`: Envia os dados de cor para os LEDs.
  
### 2. **Controle do Display OLED SSD1306**:
O display OLED é utilizado para exibir o estado dos LEDs e outros caracteres. O display é controlado via protocolo I2C e as funções do `ssd1306.c`.

- Funções principais:
  - `initialize_i2c()`: Inicializa o barramento I2C.
  - `ssd1306_init()`: Inicializa o display SSD1306.  
  - `ssd1306_draw_char()`: Desenha os caraceteres definidos no arquivo 'font.h'
  - `ssd1306_draw_string()`: Desenha strings no display chamando a função anterior.
    
### 3. **Botões e Interrupções**:
Os botões A e B são utilizados para controlar o estado dos LEDs RGB. Cada botão alterna o estado atual do LED correspondente. As interrupções de botões são tratadas com debounce para evitar múltiplas leituras de um único pressionamento.

- Funções principais:
  - `init_buttons()`: Configura os pinos GPIO para os botões.
  - `gpio_irq_handler()`: Função de interrupção que é chamada quando os botões são pressionados.
  - 
### 4. **Debounce**:
A técnica de debounce é utilizada para garantir que a leitura dos botões não seja repetida rapidamente em sucessivas interrupções. Isso é feito adicionando um atraso de 200 ms após cada pressionamento de botão.

## Bibliotecas do Código

```c
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "ws2812.pio" 
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "numeros.h"
```

## Funções Principais

### 1. **initialize_i2c()**:
Inicializa o barramento I2C para controle do display OLED SSD1306. Define os pinos GPIO 14 e 15 como SDA e SCL, respectivamente, e configura a velocidade de comunicação do I2C.

### 2. **init_buttons()**:
Configura os pinos GPIO 5 e 6 para os botões A e B. Além disso, ativa as interrupções para detectar o pressionamento dos botões e executa a técnica de debounce para evitar múltiplas leituras do mesmo evento.

### 3. **set_one_led()**:
Controla a cor dos LEDs na matriz de LEDs WS2812.

### 4. **gpio_irq_handler()**:
Função de interrupção que é chamada sempre que um dos botões A ou B é pressionado. A função aplica debounce para garantir que a leitura do botão seja feita de forma estável e que não haja múltiplas leituras do mesmo evento.

### 5. **ssd1306_init()**:
Inicializa o display OLED SSD1306 e o configura para exibir informações. Esta função é usada para garantir que o display esteja pronto para exibir os dados enviados posteriormente.

### 6. **ssd1306_draw_string()**:
Exibe uma string no display OLED SSD1306. Essa função permite atualizar a tela com texto informativo, como o status dos LEDs ou caracteres recebidos via UART-USB.


## Executando o Projeto

1. **Compilar o código**: Use o CMake para compilar o projeto.
2. **Carregar o programa** no Raspberry Pi Pico W.
3. **Conectar o Raspberry Pi Pico W** via UART-USB para monitoramento serial.

A partir de agora, você pode interagir com os LEDs RGB através dos botões, e visualizar os caracteres recebidos via UART-USB no display OLED e números na matriz de LEDs.

## Vídeo de demonstração
Link do vídeo no Youtube:https://youtube.com/shorts/NUFzkd_RD8E?feature=share

