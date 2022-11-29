#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

#define HIGH 1
#define LOW 0

#define GREEN 6
#define YELLOW 5
#define RED 4

#define DELAY1_MS 1000

#define ARRAY_LENGTH 3
int LED_PINS_ARR[3] = {GREEN, YELLOW, RED};

void set_one_to_high(int pinToSet, int ledPinsArray[], int arrayLength) {
    for (size_t i = 0; i < arrayLength; i++)
    {
        if (pinToSet == ledPinsArray[i])
        {
            gpio_set_level(ledPinsArray[i], HIGH);
            printf("Set %d to HIGH\t", ledPinsArray[i]);
        } 
        else
        {
            gpio_set_level(ledPinsArray[i], LOW);
            printf("Set %d to LOW\t", ledPinsArray[i]);
        }   
    }
    printf("\n");
}

void app_main() {
    printf("Setting GPIO direction\n");
    gpio_set_direction(GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(YELLOW, GPIO_MODE_OUTPUT);
    gpio_set_direction(RED, GPIO_MODE_OUTPUT);

    while (true)
    {
        set_one_to_high(GREEN, LED_PINS_ARR, ARRAY_LENGTH);
        vTaskDelay(DELAY1_MS / portTICK_PERIOD_MS);
        set_one_to_high(YELLOW, LED_PINS_ARR, ARRAY_LENGTH);
        vTaskDelay(DELAY1_MS / portTICK_PERIOD_MS);
        set_one_to_high(RED, LED_PINS_ARR, ARRAY_LENGTH);
        vTaskDelay(DELAY1_MS / portTICK_PERIOD_MS);
    }
}