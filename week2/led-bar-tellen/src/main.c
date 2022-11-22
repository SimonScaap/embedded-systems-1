#include <stdio.h>
#include <math.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define HIGH 1
#define LOW 0

#define DIGITS = 5

//#define LED_PINS_ARR[5] 

void app_main() {
    printf("Setting up\n");
    int led_pins_arr[5] = {GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_6, GPIO_NUM_7, GPIO_NUM_15};
    for (size_t y = 0; y < 5; y++)
    {
        gpio_set_direction(led_pins_arr[y], GPIO_MODE_OUTPUT);
    }
    
    int count = 32;
    int delay_ms = 1000; // Delay tussen het verhogen van de teller
    while (true)
    {
        for (size_t x = 0; x < count; x++)
        {
            printf("Begingetal: %d\n", x); // print begin nummer

            // Zet index i in een nieuwe variabele
            int begin_number = x;

            // For loop voor het displayen van elke bit
            for (size_t i = 0; i < 5; i++)
            {
                // Brekent de grootste macht van 2 die van het begingetal afgetrokken kan worden
                int subtract = pow(2,(5 - (i+1))); 

                // Print het getal dat afgetrokken word en het oude getal
                printf("Bit-Worth: %-3d ",subtract);

                if (begin_number - subtract  >=  0) {
                    printf("Bit: 1");
                    begin_number = begin_number - subtract; // Trekt alleen de subtract waarde af wanneer het resultaat groter dan of geloijk aan 0 is
                    gpio_set_level(led_pins_arr[i], HIGH); // Zet een led op HIGH als de bit 1 is
                } else {
                    printf("Bit: 0");
                    gpio_set_level(led_pins_arr[i], LOW);
                }
                printf(" new: %d\n", begin_number);
            }
            printf("\n");
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        }
    }
    
}