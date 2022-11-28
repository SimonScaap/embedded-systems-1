#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc.h"
#include "esp_intr_alloc.h"

#define HIGH 1
#define LOW 0
#define LOWER_BUTTON 4
#define HIGHER_BUTTON 5

int count = 0;

int ioLowerButton = 0;
int ioLowerButtonCallback = 0;

void addToCount() {
    count += 1;
    printf("Added To count\n");
    printf("Current Count: %d\n", count);
}
void subtractFromCount() {
    count -= 1;
    printf("Subtracted From count\n");
    printf("Current Count: %d\n", count);

}

void app_main() {
    // Setup
    gpio_set_direction(LOWER_BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(HIGHER_BUTTON, GPIO_MODE_INPUT);
    while (true)
    {
        ioLowerButton = gpio_get_level(LOWER_BUTTON);
        if (ioLowerButton > ioLowerButtonCallback)
        {
            subtractFromCount();
        }
        
        
        vTaskDelay(2 / portTICK_PERIOD_MS);
        ioLowerButtonCallback = ioLowerButton;

    }
    
}
