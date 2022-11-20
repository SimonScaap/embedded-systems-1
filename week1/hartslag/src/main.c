/*


*/


#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN 16
#define HIGH 1
#define LOW 0


void app_main() {

    // Setup
    printf("Hartslagmonitor is gestart\n");
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    int loop_count = 15;
    int short_delay_ms = 100;
    int long_delay_ms = 400;

    // Main loop voor de hartslag
    for (size_t i = 0; i < loop_count; i++) {
        // Bons 1 met shortdelay
        gpio_set_level(LED_PIN, HIGH);
        vTaskDelay(short_delay_ms / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, LOW);
        printf("Bons 1\n");
        vTaskDelay(short_delay_ms / portTICK_PERIOD_MS);

        // Bons 2 met shortdelay
        gpio_set_level(LED_PIN, HIGH);
        vTaskDelay(short_delay_ms / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, LOW);
        printf("Bons 2\n\n");
        vTaskDelay(short_delay_ms / portTICK_PERIOD_MS);

        // Long delay
        vTaskDelay(long_delay_ms / portTICK_RATE_MS);
    }
}

// We love C