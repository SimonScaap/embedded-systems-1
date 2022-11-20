#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
  
#define LED_PIN 21
#define HIGH 1
#define LOW 0


  
void app_main() {
    // Setup
    int delay_ms = 1500;
    int count = 10;
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    // Main loop voor het blinken
    for (size_t i = 0; i < count; i++)
    
    {
        // Zet led pin aan en delayt
        gpio_set_level(LED_PIN, HIGH);
        printf("Blinkie AAN\n");
        vTaskDelay(delay_ms / portTICK_PERIOD_MS); // 1000 milliseconde = 1 seconde
        
        // Zet led pin uit en delayt
        gpio_set_level(LED_PIN, LOW);
        printf("Blinkie UIT\n");
        vTaskDelay(delay_ms / portTICK_PERIOD_MS);

    }
    
        
} 