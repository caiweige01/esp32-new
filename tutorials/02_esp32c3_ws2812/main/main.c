#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"

#define WS2812_GPIO_NUM 2
#define WS2812_LED_COUNT 1
#define WS2812_RMT_RESOLUTION_HZ (10 * 1000 * 1000)
#define COLOR_DELAY_MS 1000

static const char *TAG = "ws2812_demo";

typedef struct {
    const char *name;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgb_color_t;

static const rgb_color_t COLORS[] = {
    {.name = "red", .red = 32, .green = 0, .blue = 0},
    {.name = "green", .red = 0, .green = 32, .blue = 0},
    {.name = "blue", .red = 0, .green = 0, .blue = 32},
};

void app_main(void)
{
    led_strip_handle_t led_strip = NULL;

    led_strip_config_t strip_config = {
        .strip_gpio_num = WS2812_GPIO_NUM,
        .max_leds = WS2812_LED_COUNT,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags = {
            .invert_out = false,
        },
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = WS2812_RMT_RESOLUTION_HZ,
        .mem_block_symbols = 64,
        .flags = {
            .with_dma = false,
        },
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    ESP_ERROR_CHECK(led_strip_clear(led_strip));

    ESP_LOGI(TAG, "WS2812 LED strip initialized on GPIO%d", WS2812_GPIO_NUM);

    while (true) {
        for (size_t i = 0; i < sizeof(COLORS) / sizeof(COLORS[0]); ++i) {
            const rgb_color_t *color = &COLORS[i];

            ESP_LOGI(TAG, "Showing %s", color->name);
            ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, color->red, color->green, color->blue));
            ESP_ERROR_CHECK(led_strip_refresh(led_strip));
            vTaskDelay(pdMS_TO_TICKS(COLOR_DELAY_MS));
        }
    }
}
