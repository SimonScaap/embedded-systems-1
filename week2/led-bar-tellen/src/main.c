#include <stdio.h>
#include <math.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define HIGH 1
#define LOW 0

#define DIGITS 10

// Delay voor de counter
#define DELAY_MS 1000

// Functie voor het checken van elke bit (nummer om te checken)
int checkDigits (int numberToCheck, int index) {
    if (((1 << index) & numberToCheck) !=0 ) {
        return 1;
    } else {
        return 0;
    }    
}

// Functie voor het aan-uit zetten van alle leds
void setLedValues (int numberToConvert, int numOfDigits, int ledPinsArr[]) {
    for (size_t i = 0; i < numOfDigits; i++)
    {
        if (checkDigits(numberToConvert, i)) {
            printf("1");
            gpio_set_level(ledPinsArr[i], HIGH); // Zet een led op HIGH als de bit 1 is
        } else {
            printf("0");
            gpio_set_level(ledPinsArr[i], LOW); // Zet een led op LOW als de bit 0 is
        }
    }
}

void app_main() {
    printf("Setting up\n");
    int led_pins_arr[DIGITS] = {GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_6, GPIO_NUM_7, GPIO_NUM_15, GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_8, GPIO_NUM_3};
    for (size_t y = 0; y < DIGITS; y++)
    {
        gpio_set_direction(led_pins_arr[y], GPIO_MODE_OUTPUT);
    }
    
    int count = pow(2, DIGITS);
    while (true)
    {
        for (size_t x = 0; x < count; x++)
        {
            printf("Begingetal: %d\n", x); // print begin nummer

            // For loop voor het displayen van elke bit
            setLedValues(x, DIGITS, led_pins_arr);
            printf("\n");
            vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
        }
    }
}

