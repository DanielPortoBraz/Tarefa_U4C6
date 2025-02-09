// ------Bibliotecas------
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "ws2812.pio.h" 
#include "hardware/irq.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "numeros.h" // Biblioteca criada para os padrões numéricos de 0 a 9 no modelo da matriz 5x5

// -----Pinos e valores padõres------

// Matriz de LEDs 5x5
#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
uint8_t led_r = 10; // Intensidade do vermelho: 20
uint8_t led_g = 0; // Intensidade do verde: 0
uint8_t led_b = 0; // Intensidade do azul: 0


// Display OLED 1306
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
ssd1306_t ssd; // Inicializa a estrutura do display

// Botões
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
// Armazena o último momento em que o botão foi pressionado
static volatile uint32_t last_time = 0; 

// LED RGB
const uint8_t leds_pins[] = {13, 11, 12}; // Ordem RGB

// UART
#define UART_ID uart0 // Seleciona a UART0
#define BAUD_RATE 115200 // Define a taxa de transmissão
#define UART_TX_PIN 0 // Pino GPIO usado para TX
#define UART_RX_PIN 1 // Pino GPIO usado para RX
static volatile char c = '0'; // Armazena o caractere de entrada

// ------Funções da matriz de LEDs ws2812------

// Buffer para armazenar quais LEDs estão ligados/desligados na matriz 5x5
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Passagem dos 32 bits de brilho do LED RGB
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Liga/desliga os LEDs da matriz na sequência
void set_one_led(bool *matriz, uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (matriz[24 - i])
        {
            put_pixel(color); // Liga o LED com 1 no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com 0 no buffer
        }
    }
}


// ------Funções do display OLED ssd1306------
void initialize_i2c(){
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
}


// ------Funções dos botões------
void init_buttons(){
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);
}

// Função callback de interrupção
void gpio_irq_handler(uint gpio, uint32_t events){
    
    // Guarda o tempo em us desde o boot do sistema
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if(current_time - last_time > 200) { // Efeito de debounce gerado pelo atraso de 200 ms na leitura do botão
        last_time = current_time;

        // Se o botão A foi pressionado e não foi atingido o número 9, incrementa-se o número exibido
        if(!gpio_get(BUTTON_A_PIN)){
            gpio_put(leds_pins[1], !gpio_get(leds_pins[1]));
            printf("BUTTON A PRESSED: LED GREEN ");

            if(gpio_get(leds_pins[1]))
                printf("ON\n");
            else
                printf("OFF\n");
        }
    
        // Se o botão B foi pressionado e não foi atingido o número 0, decrementa-se o número exibido
        else if(!gpio_get(BUTTON_B_PIN)){
            gpio_put(leds_pins[2], !gpio_get(leds_pins[2]));
            printf("BUTTON B PRESSED: LED BLUE ");

            if(gpio_get(leds_pins[2]))
                printf("ON\n");
            else
                printf("OFF\n");
        } 
    }
}   


// ------Funções do LED RGB------
void init_leds(){
    for (int i = 0; i < 3; i++){
        gpio_init(leds_pins[i]);
        gpio_set_dir(leds_pins[i], GPIO_OUT);
        gpio_put(leds_pins[i], 0);
    }
}


// ------Funções da UART------
// Inicialização
void initialize_uart(){
    // Inicializa a UART
    uart_init(UART_ID, BAUD_RATE);

    // Configura os pinos GPIO para a UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino 0 para TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pino 1 para RX

    uart_set_fifo_enabled(UART_ID, true);

    // Configurações adicionais (opcional) 
    uart_set_hw_flow(UART_ID, false, false); // Desabilita controle de fluxo 
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE); // Configura formato (8N1) 
}

// Função de callback que será chamada quando a interrupção ocorrer 
void on_uart_rx() { 
    // Enquanto houver dados para ler na UART 
    while (uart_is_readable(UART_ID)) { 
        // Lê o caractere recebido 
        c = uart_getc(UART_ID); 
        // Envia o caractere de volta (echo) para verificação 
        uart_putc(UART_ID, c); 
    } 
} 


// ======== Programa principal =========
int main()
{
    stdio_init_all();

    // Inicializações
    initialize_uart();
    initialize_i2c();
    init_buttons();
    init_leds();

    // Configuração dos LEDs ws2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Configuração do Display OLED 1306
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Interrupções dos botões A e B
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Configura a interrupção para a UART0 
    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx); // Define a função de callback para a interrupção de recepção 
    irq_set_enabled(UART0_IRQ, true); // Habilita a interrupção na UART0 
    uart_set_irq_enables(UART_ID, true, false); // Habilita a interrupção de recepção de dados (RX) na UART 
    
    // Mensagem padrão com o estado dos LEDs e indicação do espaço para caractere no display
    char *default_text[] = {"LEDS      CHAR",
        "R",
        "G",
        "B"}; 

    while (true) { 
    
        //Utilizada somente para comunicação serial via USB
        //c = getchar_timeout_us(0); // Lê o caractere sem bloquear o resto das ações

        // Escreve o estado dos leds e o espaço para receber o caractere no display
        for(int i = 0; i < count_of(default_text) ; i++){
            ssd1306_draw_string(&ssd, default_text[i], 5, i * 10);

            if (i > 0){ // Espera chegar na linha de cada LED para escrever o estado

                if(gpio_get(leds_pins[i - 1]))
                    ssd1306_draw_string(&ssd, "ON ", 20, i * 10);

                else
                    ssd1306_draw_string(&ssd, "OFF", 20, i * 10);
            }
            
            ssd1306_send_data(&ssd);
        }

        if (c - '0' < 10) // Se o caractere digitado for um algarismo
            // Desenha o número correspondente na matriz de leds
            set_one_led(nums[c - '0'], led_r, led_g, led_b);
            
        else{
            ssd1306_draw_char(&ssd, c, 100, 20);
            ssd1306_send_data(&ssd);
        }
        sleep_ms(200);
    }
}

