#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define HIGH 1
#define LOW 0
#define LOWER_BUTTON 5
#define HIGHER_BUTTON 4
#define CALLBACK_DELAY 10

void app_main() {
    // Setup
    gpio_set_direction(LOWER_BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(HIGHER_BUTTON, GPIO_MODE_INPUT);
    int getLevelLower = 0;
    int lowerCallback = 0;
    int getLevelHigher = 0;
    bool higherCallback = 0;
    int count = 0;

    while (true)
    {
        // Read State Of Both Buttons
        getLevelLower = gpio_get_level(LOWER_BUTTON);
        getLevelHigher = gpio_get_level(HIGHER_BUTTON);
        
        // Compare buttons to previous value
        if (getLevelLower > lowerCallback)
        {
            count++;
            
        }
        if (getLevelHigher > higherCallback)
        {
            count--;
        }
        
        // Als count < 0 dan word het op 0 gezet
        if (count < 0)
        {
            count = 0;
        }

        // Print Button State
        printf("Hello World!  ");
        printf("Knopje: %d  %d | count: %d\n", getLevelLower, getLevelHigher, count);
        vTaskDelay(CALLBACK_DELAY / portTICK_PERIOD_MS);

        // Set callback
        lowerCallback = getLevelLower;
        higherCallback = getLevelHigher;

    }
    
    
}
