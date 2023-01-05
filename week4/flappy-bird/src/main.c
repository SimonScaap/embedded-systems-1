/*
Met dit programma kun je het flappy bird spel op de DLO spelen. Als je op de knop drukt, dat word het commando ">" verstuurd.
@author Simon Zweers
@date 12 12 2022
*/

#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/uart.h"

#define BUTTON_INPUT 4

void app_main() {
    gpio_set_direction(BUTTON_INPUT, GPIO_MODE_INPUT);
    int buttonValue = 0;
    int buttonValueCallback = 0;

    while (true)
    {
        buttonValue = gpio_get_level(BUTTON_INPUT);
        if (buttonValue > buttonValueCallback)
        {
            printf(">\n");        
        }
        buttonValueCallback = buttonValue;
        vTaskDelay(20/portTICK_PERIOD_MS);
    }
}