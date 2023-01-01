/*
Dit programma stuurt een stoplicht aan door commando's te sturen over UART
@author Simon Zweers
@date 22 12 2022
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define BUF_SIZE (1024)

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

#define LEDC_FREQ 5000
#define LEDC_MAX_DUTY (8192 - 1)

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

void blink_orange_led(void *p) {
    //configASSERT(((uint32_t)p) == 1);
    set_one_to_high(YELLOW, LED_PINS_ARR, ARRAY_LENGTH);
    while (true)
    {
        
        gpio_set_level(YELLOW, HIGH);
        vTaskDelay(750 / portTICK_PERIOD_MS);
        gpio_set_level(YELLOW, HIGH);
        vTaskDelay(750 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    // Set gpio direction
    for (size_t i = 0; i < ARRAY_LENGTH; i++)
    {
        gpio_set_direction(LED_PINS_ARR[i], GPIO_MODE_OUTPUT);
    }
    
    // UART config
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // databuffer aanmaken
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    // TASK handles
    TaskHandle_t xHandle1 = NULL;
    
    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(UART_NUM_0, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
    
        if (len) {
            data[len] = '\0';
            printf("> %s\n", data);

            switch (data[0]) {
            case '1':
                printf("command: 1\n");
                set_one_to_high(RED, LED_PINS_ARR, ARRAY_LENGTH);
                break;
            case '2':
                printf("command: 2\n");
                set_one_to_high(YELLOW, LED_PINS_ARR, ARRAY_LENGTH);
                break;
            case '3':
                printf("command: 3\n");
                set_one_to_high(GREEN, LED_PINS_ARR, ARRAY_LENGTH);
                break;
            case 'A':
                xTaskCreate(
                    blink_orange_led,
                    "Name1",
                    5000,
                    NULL,
                    tskIDLE_PRIORITY,
                    xHandle1
                );
                break;

            default:
                break;
            }
        }
    }
    
    xTaskCreate(
        blink_orange_led,
        "Name1",
        5000,
        NULL,
        tskIDLE_PRIORITY,
        xHandle1
    );
}
