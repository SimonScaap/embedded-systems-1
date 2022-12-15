#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define HIGH 1
#define LOW 0
#define POTENTIOMETER ADC1_CHANNEL_3 // GPIO 4
#define ARRAY_SIZE 10

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
        if (checkDigits(1 << numberToConvert, i)) {
            printf("1");
            gpio_set_level(ledPinsArr[i], HIGH); // Zet een led op HIGH als de bit 1 is
        } else {
            printf("0");
            gpio_set_level(ledPinsArr[i], LOW); // Zet een led op LOW als de bit 0 is
        }
    }
}


void app_main() {
    int ledPinsArray[] = {5, 6, 7, 15, 16, 17, 18, 8, 3, 46};
    for (size_t i = 0; i < ARRAY_SIZE; i++)
    {
        gpio_set_direction(ledPinsArray[i], GPIO_MODE_OUTPUT);
    }
    
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(POTENTIOMETER, ADC_ATTEN_DB_11);


    int adc1Read;
    int ledOnValue = 0;
    /*
    while (true)
    {
        adc1Read = adc1_get_raw(POTENTIOMETER);
        printf("Raw adc read: %d\n", adc1Read);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    */
    while (true)
    {
        for (size_t i = 0; i < (ARRAY_SIZE - 1); i++)
        {
            adc1Read = adc1_get_raw(POTENTIOMETER);

            setLedValues(ledOnValue, ARRAY_SIZE, ledPinsArray);
            printf("Index %d \n", ledOnValue);
            ledOnValue = ledOnValue + 1;

            vTaskDelay((adc1Read / 10) / portTICK_PERIOD_MS);
        }
        for (size_t i = 0; i < (ARRAY_SIZE - 1); i++)
        {
            adc1Read = adc1_get_raw(POTENTIOMETER);
            
            setLedValues(ledOnValue, ARRAY_SIZE, ledPinsArray);
            printf("Index %d \n", ledOnValue);
            ledOnValue = ledOnValue - 1;

            vTaskDelay((adc1Read / 10) / portTICK_PERIOD_MS);
        }   
    }
}