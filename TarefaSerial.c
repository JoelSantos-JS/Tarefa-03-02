#include <stdio.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "incre/ssd1306.h"
#include "matriz_leds.h"

#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12
#define BUTTON_A 5
#define BUTTON_B 6

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define DISPLAY_ADDR 0x3C

ssd1306_t display;
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;

const Matriz_leds_config zero = {
    // Verde médio 
    {{0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.5, 0.0
    
    }, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}}
};
const Matriz_leds_config one = {
    // Azul médio com formato tradicional do número 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0 - parte superior do 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}}  // Linha 4 - base do 1
};
const Matriz_leds_config two = {
    // Amarelo médio 
    {{0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}}
};

const Matriz_leds_config three = {
    // Magenta médio 
    {{0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.0, 0.0, 0.0}}
};

const Matriz_leds_config four = {
    // Ciano médio 
    {{0.0, 0.5, 0.5}, {0.0, 0.5, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.5, 0.5}, {0.0, 0.5, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.5, 0.5}, {0.0, 0.5, 0.5}, {0.0, 0.5, 0.5}, {0.0, 0.5, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.5}, {0.0, 0.0, 0.0}}
};

const Matriz_leds_config five = {
    // Branco médio 
    {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}},
    {{0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}},
    {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}}
};

const Matriz_leds_config six = {
    // Vermelho médio
    {{0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}}
};

const Matriz_leds_config seven = {
    // Verde médio
    {{0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
};

const Matriz_leds_config eight = {
    // Azul médio
    {{0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}}
};

const Matriz_leds_config nine = {
    // Amarelo médio
    {{0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
};
const Matriz_leds_config *nums[] = {&zero, &one, &two, &three, &four, &five, &six, &seven, &eight, &nine};


void init_gpios() {
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
}
// Inicializa display
void init_display() {
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    ssd1306_init(&display, WIDTH, HEIGHT, false, DISPLAY_ADDR, I2C_PORT);
    ssd1306_config(&display);
}



// Função LED vermelho
void blink_red_led() {
    gpio_put(LED_RED, 1);   // Correto, mantém padrão
    sleep_ms(100);
    gpio_put(LED_RED, 0);
    sleep_ms(100);
}

// Função de callback para os botões
void button_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A) {
        gpio_put(LED_GREEN, !gpio_get(LED_GREEN));
        ssd1306_fill(&display, false);
        ssd1306_draw_string(&display, "LED Verde:", 10, 10);
        ssd1306_draw_string(&display, gpio_get(LED_GREEN) ? "ON" : "OFF", 10, 25);
        ssd1306_send_data(&display);
        printf("LED Verde: %s\n", gpio_get(LED_GREEN) ? "ON" : "OFF");
        button_a_pressed = true;
    } 
    else if (gpio == BUTTON_B) {
        gpio_put(LED_BLUE, !gpio_get(LED_BLUE));
        ssd1306_fill(&display, false);
        ssd1306_draw_string(&display, "LED Azul:", 10, 10);
        ssd1306_draw_string(&display, gpio_get(LED_BLUE) ? "ON" : "OFF", 10, 25);
        ssd1306_send_data(&display);
        printf("LED Azul: %s\n", gpio_get(LED_BLUE) ? "ON" : "OFF");
        button_b_pressed = true;
    }
}


void handle_uart(char c, PIO pio, uint sm) {
    printf("Caractere recebido: %c\n", c);

    // Mostra no display
    ssd1306_fill(&display, false);
    ssd1306_draw_string(&display, "Recebido:", 10, 10);
    ssd1306_draw_char(&display, c, 10, 30);
    ssd1306_send_data(&display);

    // Se for número, mostra na matriz
    if(isdigit(c)) {
        int num = c - '0';
        imprimir_desenho(nums[num], pio, sm);
    }
}

// Função de debounce
void debounce(int delay_ms) {
    sleep_ms(delay_ms);
}

// Função para atualizar o índice do número
void update_index(bool increment, int *i) {
    if (increment && *i < 9) {
        (*i)++;
    } else if (!increment && *i > 0) {
        (*i)--;
    }
}

// Função principal
int main() {
    stdio_init_all();
    
    // Aguarda conexão USB (opcional, mas recomendado)
   
    init_gpios();
    init_display();

    PIO pio = pio0;
    uint sm = configurar_matriz(pio);

    ssd1306_fill(&display, false);
    ssd1306_draw_string(&display, "Sistema", 10, 10);
    ssd1306_draw_string(&display, "Iniciado!", 10, 25);
    ssd1306_send_data(&display);

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

    while(true) {

        // Verifica entrada serial
        if(stdio_usb_connected()) {
            char c;
            if(scanf("%c", &c) == 1) {
                handle_uart(c, pio, sm);
            }
        }

        if(button_a_pressed) {
            button_a_pressed = false;
            sleep_ms(200);  // debounce
        }
        
        if(button_b_pressed) {
            button_b_pressed = false;
            sleep_ms(200);  // debounce
        }

        sleep_ms(10);
    }

    return 0;
}