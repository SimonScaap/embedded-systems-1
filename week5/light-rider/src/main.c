#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define POTENTIOMETER ADC1_CHANNEL_3

void app_main() {
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(POTENTIOMETER, ADC_ATTEN_DB_11);

    int adc1Read;
    
    while (true)
    {
        adc1Read = adc1_get_raw(POTENTIOMETER);
        printf("Raw adc read: %d\n", adc1Read);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    

}