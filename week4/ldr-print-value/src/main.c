/*
Dit programma print de waarde van een LDR in de console d.m.v. ADC
@author Simon Zweers
@date 12 12 2022
*/

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <driver/adc.h>
#include <driver/uart.h>

#define LDR_PIN ADC1_CHANNEL_0
#define LED_PIN GPIO_NUM_2
#define THRESHOLD 100
#define HIGH 1
#define LOW 0

void app_main() {
    gpio_set_direction(LDR_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(LDR_PIN, ADC_ATTEN_DB_11);
    
    int rawAdc;

    while (true)
    {
        rawAdc = adc1_get_raw(ADC1_CHANNEL_0);
        printf("Raw analog value: %d\n", rawAdc);
        if (rawAdc < THRESHOLD)
        {
            gpio_set_level(LED_PIN, HIGH);
        } else
        {
            gpio_set_level(LED_PIN, LOW);
        }
        
        
        vTaskDelay(100/ portTICK_PERIOD_MS);
        
    }
    
}