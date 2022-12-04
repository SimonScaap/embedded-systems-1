#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

#define HIGH 1
#define LOW 0

#define GREEN 6
#define YELLOW 5
#define RED 4
#define BUTTON_INPUT 18

// Delays voor stoplichtcycle
#define DELAY1_MS 1000
#define TRAFFIC_DELAY_MS 3000

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

void traffic_light_cycle() {
    // GREEN
    vTaskDelay(TRAFFIC_DELAY_MS / portTICK_PERIOD_MS);
    set_one_to_high(GREEN, LED_PINS_ARR, ARRAY_LENGTH);
    printf("turned on GREEN\n");
    vTaskDelay(TRAFFIC_DELAY_MS / portTICK_PERIOD_MS);
    // YELLOW
    set_one_to_high(YELLOW, LED_PINS_ARR, ARRAY_LENGTH);
    printf("turned on YELLOW\n");
    vTaskDelay(TRAFFIC_DELAY_MS / portTICK_PERIOD_MS);
    // RED
    set_one_to_high(RED, LED_PINS_ARR, ARRAY_LENGTH);
    printf("turned on RED\n");
}

void app_main() {
    printf("Setting GPIO direction\n");
    gpio_set_direction(GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(YELLOW, GPIO_MODE_OUTPUT);
    gpio_set_direction(RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_INPUT, GPIO_MODE_INPUT);
    int buttonValue = 0;
    int buttonValueCallback = 0;
    int tempCount = 0;

    bool breakLoop = false;
    while (true)
    {
        set_one_to_high(RED, LED_PINS_ARR, ARRAY_LENGTH);
        while (!breakLoop)
        {
            
            buttonValue = gpio_get_level(BUTTON_INPUT);
            if (buttonValue > buttonValueCallback)
            {
                tempCount += 1;
                printf("Button Pressed! : %d\n", tempCount);
                breakLoop = true;
            }
            buttonValueCallback = buttonValue;
            
            vTaskDelay(10/ portTICK_PERIOD_MS);
            
        }
        
        
        printf("ButtonDelay\n");
        vTaskDelay(TRAFFIC_DELAY_MS / portTICK_PERIOD_MS);
        traffic_light_cycle();
        breakLoop = false;
    }
    
}