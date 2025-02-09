# Projeto Raspberry Pi Pico W - Interfaces de Comunicação I2C e UART

Este projeto tem como objetivo proporcionar o aprendizado dos conceitos de comunicação serial I2C e UART no microcontrolador RP2040, além de demonstrar o uso de interrupções e debounce. Utiliza-se o SDK Pico C/C++ para o desenvolvimento e manipulação de periféricos como LEDs RGB, display OLED e botões. Para uso dos periféricos e aplicação prática, utiliza-se a placa BitDogLab. Contudo, o projeto pode ser simulado através da extensão Wokwi, para isso, veja os vídeos de demonstração. 

Obs: Antes de executar o código na placa BitDogLab, leia o tópico **BitDogLab: UART X USB**.

## Funcionalidades Principais

- **Matriz de LEDs RGB 5x5**: Controla uma matriz de LEDs para exibir o número recebido pela comunicação serial.
- **Display OLED SSD1306**: Exibe informações sobre o estado dos LEDs e o caractere de entrada.
- **Botões**: Controla LEDs RGB com dois botões físicos, A e B, com debounce para evitar leituras falsas.
- **Comunicação UART**: Envia e recebe dados via UART para comunicação serial.
- **Interrupções**: Gerencia interrupções de botões e UART para uma comunicação eficiente.

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

- **UART**:
  - Pino GPIO 0 (TX).
  - Pino GPIO 1 (RX).
  - Taxa de transmissão: 115200 bauds.

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

### 5. **Comunicação UART**:
A comunicação UART é utilizada para receber e enviar dados. O caractere recebido via UART é exibido no display OLED.

- Funções principais:
  - `initialize_uart()`: Inicializa a UART com a taxa de transmissão configurada.
  - `on_uart_rx()`: Função callback para ler dados recebidos pela UART.

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
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "numeros.h"
```

## Funções Principais

### 1. **initialize_uart()**: 
Inicializa a comunicação UART. Configura os pinos de TX (GPIO 0) e RX (GPIO 1) e define a taxa de transmissão como 115200 bauds. Esta função permite a comunicação serial entre o Raspberry Pi Pico W e um dispositivo externo via UART.

### 2. **initialize_i2c()**:
Inicializa o barramento I2C para controle do display OLED SSD1306. Define os pinos GPIO 14 e 15 como SDA e SCL, respectivamente, e configura a velocidade de comunicação do I2C.

### 3. **init_buttons()**:
Configura os pinos GPIO 5 e 6 para os botões A e B. Além disso, ativa as interrupções para detectar o pressionamento dos botões e executa a técnica de debounce para evitar múltiplas leituras do mesmo evento.

### 4. **set_one_led()**:
Controla a cor dos LEDs na matriz de LEDs WS2812.

### 5. **gpio_irq_handler()**:
Função de interrupção que é chamada sempre que um dos botões A ou B é pressionado. A função aplica debounce para garantir que a leitura do botão seja feita de forma estável e que não haja múltiplas leituras do mesmo evento.

### 6. **ssd1306_init()**:
Inicializa o display OLED SSD1306 e o configura para exibir informações. Esta função é usada para garantir que o display esteja pronto para exibir os dados enviados posteriormente.

### 7. **ssd1306_draw_string()**:
Exibe uma string no display OLED SSD1306. Essa função permite atualizar a tela com texto informativo, como o status dos LEDs ou caracteres recebidos via UART.

### 8. **on_uart_rx()**:
Função callback chamada sempre que dados são recebidos pela UART. Os dados são exibidos no display OLED e também enviados de volta para o dispositivo, realizando um eco dos caracteres recebidos.

## ❗ BitDogLab: UART X USB
O código apresentado suporta aplicação para **UART** e **USB**, contudo a aplicação para UART funciona apenas para a simulação no **Wokwi**, ou com um **cabo USB-UART** adequado. Devido à falta deste cabo, a execução na placa **BitDogLab** ficou restringida à prática com monitoriamento serial **via USB**, causando o funcionamento distinto em comparação à comunicação UART.

### Execução: UART X USB
A diferença se trata da exibição do caractere digitado, visto que através da **UART**, há a leitura por interrupção e o caractere permanece no display até que outro seja enviado. Já pela **USB**, o caractere é exibido por um período de 200 ms no display pela função `getchar_timeout_us()` que destina um período determinado à leitura do caractere, isso é feito para não bloquear o loop principal (efeito provocado pelo `if(scanf("%c", &c´) ==1 )`).

Obs: Caso utilize o modelo **USB**, retire do comentário, a linha 223:
```c
223 //c = getchar_timeout_us(0); // Lê o caractere sem bloquear o resto das ações
```

## Executando o Projeto

1. **Compilar o código**: Use o CMake para compilar o projeto.
2. **Carregar o programa** no Raspberry Pi Pico W.
3. **Conectar o Raspberry Pi Pico W** via USB-UART para monitoramento serial.

A partir de agora, você pode interagir com os LEDs RGB através dos botões, e visualizar os caracteres recebidos via UART no display OLED e números na matriz de LEDs.

## Vídeo de demonstração
Link do vídeo no youtube:

