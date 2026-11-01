#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

constexpr gpio_num_t BUTTON_PIN = GPIO_NUM_10;   // пин, с которого читаем кнопку
constexpr gpio_num_t GND_PIN    = GPIO_NUM_8;    // пин-"земля" для кнопки: всегда 0

extern "C" void app_main(void)
{
    // Пин 8: выход, на нём всегда 0 (он заменяет провод GND)
    gpio_config_t out_conf = {};
    out_conf.pin_bit_mask = (1ULL << GND_PIN);
    out_conf.mode         = GPIO_MODE_OUTPUT;
    out_conf.pull_up_en   = GPIO_PULLUP_DISABLE;
    out_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    out_conf.intr_type    = GPIO_INTR_DISABLE;
    gpio_config(&out_conf);
    gpio_set_level(GND_PIN, 0);

    // Пин 10: вход с подтяжкой вверх (кнопка отпущена = 1, нажата = 0)
    gpio_config_t in_conf = {};
    in_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
    in_conf.mode         = GPIO_MODE_INPUT;
    in_conf.pull_up_en   = GPIO_PULLUP_ENABLE;
    in_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    in_conf.intr_type    = GPIO_INTR_DISABLE;
    gpio_config(&in_conf);

    int last = gpio_get_level(BUTTON_PIN);           // прошлое состояние кнопки
    printf("Старт. Кнопка %s\n", last == 0 ? "нажата" : "отпущена");

    while (true) {
        int now = gpio_get_level(BUTTON_PIN);        // текущее состояние

        if (now != last) {                           // печатаем только при смене
            if (now == 0) {
                printf("Кнопка нажата\n");
            } else {
                printf("Кнопка отпущена\n");
            }
            last = now;
        }

        vTaskDelay(pdMS_TO_TICKS(20));               // пауза 20 мс (и от дребезга, и для системы)
    }
}