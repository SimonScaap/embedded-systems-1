#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define HIGH 1
#define LOW 0
#define LOWER_BUTTON 5
#define HIGHER_BUTTON 4
#define CALLBACK_DELAY 10
#define LED_BAR_LENGTH 10

int checkDigits (int numberToCheck, int index) {
    if (((1 << index) & numberToCheck) !=0 ) {
        return 1;
    } else {
        return 0;
    }    
}

void setLedValues (int numberToConvert, int numOfDigits, int ledPinsArr[]) {
    for (size_t i = 0; i < numOfDigits; i++)
    {
        if (numberToConvert > i) {
            printf("1");
            gpio_set_level(ledPinsArr[i], HIGH); // Zet een led op HIGH als de bit 1 is
        } else {
            printf("0");
            gpio_set_level(ledPinsArr[i], LOW); // Zet een led op LOW als de bit 0 is
        }
    }
}

void app_main() {
    // Setup
    gpio_set_direction(LOWER_BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(HIGHER_BUTTON, GPIO_MODE_INPUT);
    int ledBarPinArray[LED_BAR_LENGTH] = {15, 16, 17, 18, 8, 3, 46, 9, 10, 11};
    for (size_t i = 0; i < LED_BAR_LENGTH; i++)
    {
        gpio_set_direction(ledBarPinArray[i], GPIO_MODE_OUTPUT);
    }
    
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

        // Zet led pins po count
        setLedValues(count, LED_BAR_LENGTH, ledBarPinArray);

        // Print Button State
        printf("Hello World!  ");
        printf("Knopje: %d  %d | count: %d\n", getLevelLower, getLevelHigher, count);
        vTaskDelay(CALLBACK_DELAY / portTICK_PERIOD_MS);

        // Set callback
        lowerCallback = getLevelLower;
        higherCallback = getLevelHigher;

    }  
}
