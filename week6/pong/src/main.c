#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BAR_SIZE 10
int ledBarArray[BAR_SIZE] = {4, 5, 6, 7, 15, 16, 17, 18, 8, 3};

#define LIVES_SIZE 5
int livesArray[LIVES_SIZE] = {10, 11, 12, 13, 14};
#define BUTTON_PLAY 46
#define ZONE_LED 9

int counter = 0;

typedef struct
{
    int buttonPin;
    int cursorIndex;
    int lives;
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

void valid_click(int led, int zoneSize, int cursor) {
    bool on = false;
    for (size_t i = 0; i < zoneSize; i++)
    {
        if (((1 << i) & cursor) != 0)
        {
            on = true;
        }
    }
    if (on)
    {
        gpio_set_level(led, 1);
    }
    else
    {
        gpio_set_level(led, 0);
    }
    
    
}

void vPressCheck(void *p) {
    ButtonPressData *data = (ButtonPressData*)p;
    int button = data->buttonPin;
    int cursorindex = data->cursorIndex;

    //int* pointer = (int*)p;
    while (true)
    {
        counter++;
        printf("Dit zijn de parameters: %d, %d - %d\n", button, cursorindex, counter);
        vTaskDelay(1500 / portTICK_PERIOD_MS);
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
    gpio_set_direction(ZONE_LED, GPIO_MODE_OUTPUT);

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
    int delay = 120;
    int delayDecay = 3;
    int score = 0;
    
    printf("ledbarIndex: %d\n", cursorIndex);
    
    // Button Task aanmaken
    TaskHandle_t xButtonThreadHandle = NULL;
    //int buttonCache;
    ButtonPressData buttonData;
    buttonData.buttonPin = BUTTON_PLAY;
    buttonData.cursorIndex = 0;
    //int parameter = 50;
    void* buttonDataPtr; 
    buttonDataPtr = &buttonData;
    printf("Mem addr parameter: %d\n\n\n", (int)&buttonData);
    xTaskCreate(vPressCheck, "Button", 5000, buttonDataPtr, tskIDLE_PRIORITY, &xButtonThreadHandle);
 
    // Led bar heen en weer
    while (!gameOver)
    {
        // Heen
        for (size_t i = 0; i < (BAR_SIZE - 1); i++)
        {
            cursorIndex = cursorIndex << 1;
            //printf("Led number: %d\n", cursorIndex);
            update_ledbar(cursorIndex, ledBarArray, BAR_SIZE);
            valid_click(ZONE_LED, 3, cursorIndex);
            vTaskDelay(delay / portTICK_PERIOD_MS);
        }
        // Terug
        for (size_t i = 0; i < (BAR_SIZE - 1); i++)
        {
            cursorIndex = cursorIndex >> 1;
            //printf("Led number: %d\n", cursorIndex);
            update_ledbar(cursorIndex, ledBarArray, BAR_SIZE);
            valid_click(ZONE_LED, 3, cursorIndex);
            vTaskDelay(delay / portTICK_PERIOD_MS);
        }

        update_lives(lives, livesArray, LIVES_SIZE);
        if (lives <= 0)
        {
            gameOver = true;
        }
        else
        {
            //vTaskDelay(1000 / portTICK_PERIOD_MS);
            printf("Loop Ended\n Lives: %d\n", lives);
            score++;
            if (delay > 50)
            {
                delay = delay - delayDecay;
            }
        }    
    }
}