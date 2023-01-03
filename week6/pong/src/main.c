#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BAR_SIZE 10
int ledBarArray[BAR_SIZE] = {4, 5, 6, 7, 15, 16, 17, 18, 8, 3};

#define LIVES_SIZE 5
int livesArray[LIVES_SIZE] = {10, 11, 12, 13, 14};

typedef struct
{
    int buttonPin;
    int cursorIndex;
} ButtonPressData;


void update_lives(int score, int livesArray[], int totalLives) {
    for (size_t i = 0; i < totalLives; i++)
    {
        if (score > i)
        {
            gpio_set_level(livesArray[i], 1);
            printf("Set pin %d to HIGH\n", livesArray[i]);
        }
        else
        {
            gpio_set_level(livesArray[i], 0);
            printf("Set pin %d to LOW\n", livesArray[i]);
        }   
    }
}

void update_ledbar(int ledIndex, int ledBarArray[], int totalLeds) {
    for (size_t i = 0; i < totalLeds; i++)
    {

        if (ledIndex == 1 << i)
        {
            gpio_set_level(ledBarArray[i], 1);
        }
        else
        {
            gpio_set_level(ledBarArray[i], 0);
        }
    }
}

void vPressCheck(void *p) {
    //ButtonPressData *data = (ButtonPressData*) pvParameters;
    int pointer = (int)(int*)p;
    while (true)
    {
        printf("Dit zijn de parameters: %d\n", pointer);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
}

void app_main() {
    
    // Zet alle Led pins op GPIO_MODE OUTPUT
    for (size_t i = 0; i < BAR_SIZE; i++)
    {
        gpio_set_direction(ledBarArray[i], GPIO_MODE_OUTPUT);
        gpio_set_level(ledBarArray[i], 1);
    }
    for (size_t i = 0; i < LIVES_SIZE; i++)
    {
        gpio_set_direction(livesArray[i], GPIO_MODE_OUTPUT);
        gpio_set_level(livesArray[i], 1);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Geeft alle levens van 0-5 weer op de leds
    for (size_t i = 0; i < (LIVES_SIZE + 1); i++)
    {
        update_lives(i, livesArray, LIVES_SIZE);
        printf("Setting Score: %d\n", i);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    // Game variabelen
    bool gameOver = false;
    int lives = 5;
    int cursorIndex = 0b01;
    
    printf("ledbarIndex: %d\n", cursorIndex);
    
    /* Task maken
    
    TaskHandle_t xButtonThreadHandle = NULL;
    int parameter = 50;
    int* parameterPtr; 
    parameterPtr = &parameter;
    printf("Mem addr parameter: %d\n\n\n", (int)&parameter);
    xTaskCreate(vPressCheck, "Button", 5000, (void *)parameter, tskIDLE_PRIORITY, &xButtonThreadHandle);

    
    */ 
    
    // Led bar heen en weer
    
    while (!gameOver)
    {
        // Heen
        for (size_t i = 0; i < (BAR_SIZE - 1); i++)
        {
            cursorIndex = cursorIndex << 1;
            printf("Led number: %d\n", cursorIndex);
            update_ledbar(cursorIndex, ledBarArray, BAR_SIZE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        // Terug
        for (size_t i = 0; i < (BAR_SIZE - 1); i++)
        {
            cursorIndex = cursorIndex >> 1;
            printf("Led number: %d\n", cursorIndex);
            update_ledbar(cursorIndex, ledBarArray, BAR_SIZE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        
        lives--;
        update_lives(lives, livesArray, LIVES_SIZE);
        if (lives <= 0)
        {
            gameOver = true;
        }
        else
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            printf("Loop Ended\n Lives: %d\n", lives);
        }        
    }
}