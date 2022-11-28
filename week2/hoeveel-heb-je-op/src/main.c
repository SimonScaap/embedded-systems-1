#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc.h"

#define HIGH 1
#define LOW 0
#define LOWER_BUTTON 4
#define HIGHER_BUTTON 5

int count = 0;

void addToCount() {
    count += 1;
    printf("Added To count\n");
}
void subtractFromCount() {
    count -= 1;
    printf("Subtracted From count\n");
}

void app_main() {
    // Setup
    gpio_set_direction(LOWER_BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(HIGHER_BUTTON, GPIO_MODE_INPUT);

    gpio_intr_enable(HIGHER_BUTTON);
    gpio_set_intr_type(HIGHER_BUTTON, GPIO_INTR_POSEDGE);
    //gpio_isr_register(addToCount, void, ESP_INTR_FLAG_LEVEL1,NULL);

    for (size_t i = 0; i < 4; i++)
    {
        addToCount();
        printf("Hello World!\n");
        printf("Blink\n");
        printf("index: %d \t count: %d\n", i, count);
    }
    
}
