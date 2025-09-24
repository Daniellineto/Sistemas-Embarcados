#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Definições
#define LED1 GPIO_NUM_21
#define LED2 GPIO_NUM_19
#define LED3 GPIO_NUM_18
#define LED4 GPIO_NUM_17
#define DELAY_MS 500

// Array dos LEDs para facilitar controle
gpio_num_t leds[4] = {LED1, LED2, LED3, LED4};

// Inicializa os LEDs como saída e apaga todos
void leds_init(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED1) | (1ULL << LED2) | (1ULL << LED3) | (1ULL << LED4),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    for (int i = 0; i < 4; i++) {
        gpio_set_level(leds[i], 0);
    }
}

// Liga ou desliga LED específico
void led_set(int index, int level) {
    if (index < 0 || index > 3) return;
    gpio_set_level(leds[index], level);
}

// Fase 1: Contador binário de 0 a 15
void fase1_contador_binario(void) {
    for (int val = 0; val < 16; val++) {
        for (int i = 0; i < 4; i++) {
            led_set(i, (val >> i) & 1);
        }
        printf("Fase 1 - Valor binário: %d\n", val);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
}

// Fase 2: Varredura vai e volta
void fase2_varredura(int ciclos) {
    int index = 0;
    int direction = 1;
    int passos_por_ciclo = 6; // 4 LEDs ida + 2 passos voltando = 6 passos por ciclo
    int total_passos = passos_por_ciclo * ciclos;

    for (int passo = 0; passo < total_passos; passo++) {
        // Apaga todos LEDs
        for (int i = 0; i < 4; i++) {
            led_set(i, 0);
        }

        // Acende o LED atual
        led_set(index, 1);
        printf("Fase 2 - LED aceso: %d\n", index + 1);

        // Atualiza índice e direção
        index += direction;
        if (index == 3) direction = -1;
        else if (index == 0) direction = 1;

        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
}

void app_main(void) {
    leds_init();

    while (true) {
        fase1_contador_binario();     // Fase 1
        fase2_varredura(1);           // Fase 2 
    }
}
