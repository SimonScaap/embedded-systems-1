#include <stdio.h>
#include <math.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define HIGH 1
#define LOW 0
#define LED_PIN 4

void app_main() {
    printf("Setting up\n");
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    
    gpio_set_level(GPIO_NUM_4, HIGH);
    printf("Set pin 4 to HIGH\n");

    int digits = 4;
    int begin_number = 6;
    

    for (size_t i = 0; i < digits; i++)
    {
        // Brekent de grootste macht van 2 die van het begingetal afgetrokken kan worden
        int subtract = pow(2,(digits - (i+1)));  

        // Print het getal dat afgetrokken word en het oude getal
        printf("Subtract: %-3d old: %-3d", subtract, begin_number);

        if (begin_number - subtract  >=  0) {
            printf("1");
            begin_number = begin_number - subtract; // Trekt alleen de subtract waarde af wanneer het resultaat groter dan of geloijk aan 0 is
        } else {
            printf("0");
        }
        printf(" new: %d\n", begin_number);
    }
    printf("\n");

}