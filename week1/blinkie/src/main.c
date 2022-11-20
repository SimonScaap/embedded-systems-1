#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
  
#define LED_PIN 21
#define HIGH 1
#define LOW 0
  
void app_main() {
    // Setup
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    // Main loop voor het blinken
    while (true) {
        // Zet led pin aan en delayt
        gpio_set_level(LED_PIN, HIGH);
        printf("Blinkie AAN\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1000 milliseconde = 1 seconde
        
        // Zet led pin uit en delayt
        gpio_set_level(LED_PIN, LOW);
        printf("Blinkie UIT\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
    
        
} 