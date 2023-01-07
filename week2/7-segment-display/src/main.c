#include <stdio.h>
#include "SevSeg.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"


void app_main() {
    uint8_t numDigits = 4;
    uint8_t digitPins[] = {39, 38, 1, 2};
    uint8_t segmentPins[] = {37, 36, 35, 0, 45, 48, 47, 21};
    
    
    sevseg_begin(numDigits, digitPins, segmentPins);
    sevseg_setChars("8888");
    
    while (true)
    {
        for (size_t i = 0; i < 100; i++)
        {
            sevseg_setNumber(i, 0, false);
            printf("number: %d\n", i);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        
    }
    
}