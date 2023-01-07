#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "SevSeg.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BAR_SIZE 10
int ledBarArray[BAR_SIZE] = {4, 5, 6, 7, 15, 16, 17, 18, 8, 3};

#define LIVES_SIZE 5
int livesArray[LIVES_SIZE] = {10, 11, 12, 13, 14};
#define BUTTON_PLAY 46
#define ZONE_LED 9
#define INITIAL_DELAY 120
#define DECAY 3
#define MIN 0
#define MAX 15

    // Game variabelen
bool buttonClicked = false;
bool gameOver = false;
bool earnedPoint = false;
int lives = 5;
int cursorIndex = 0b01;
int baseDelay = INITIAL_DELAY;
int randomDelayOffset;
int delayDecay = DECAY;
int score = 0;

void update_lives(int score, int livesArray[], int totalLives) {
    for (size_t i = 0; i < totalLives; i++)
    {
        if (score > i)
        {
            gpio_set_level(livesArray[i], 1);
        }
        else
        {
            gpio_set_level(livesArray[i], 0);
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
        // Als Je kan klikken gaat de zone led aan
    if (on)
    {
        gpio_set_level(led, 1);
    }
    else
    {
        gpio_set_level(led, 0);
    }

        // Als de knop ingedrukt is terwijl dat moet dan gaat er geen leven vanaf
    if (gpio_get_level(BUTTON_PLAY)) 
    {
        if (on)
        {

            buttonClicked = true;
            earnedPoint = true;
        }
        else {
            buttonClicked = true;
            lives--;
            earnedPoint = false;
        }
    }
}

void game_loop(){
    update_lives(LIVES_SIZE, livesArray, LIVES_SIZE);
    sevseg_setNumber(score, -1, false);

    while (!gameOver)
    {
        randomDelayOffset = rand()%((MAX+MIN)-1) + MIN;
        earnedPoint = false;
        // Heen
        for (size_t i = 0; i < (BAR_SIZE - 1); i++)
        {
            cursorIndex = cursorIndex << 1;
            update_ledbar(cursorIndex, ledBarArray, BAR_SIZE);
            update_lives(lives, livesArray, LIVES_SIZE);                    
            vTaskDelay((baseDelay + randomDelayOffset) / portTICK_PERIOD_MS);
        }

        // Terug
        for (size_t i = 0; i < (BAR_SIZE - 1); i++)
        {
            cursorIndex = cursorIndex >> 1;
            update_ledbar(cursorIndex, ledBarArray, BAR_SIZE);
            valid_click(ZONE_LED, 3, cursorIndex);
            update_lives(lives, livesArray, LIVES_SIZE);
            vTaskDelay((baseDelay + randomDelayOffset) / portTICK_PERIOD_MS);
        }
        // Zet de groene led weer uit
        gpio_set_level(ZONE_LED, 0);
        if (buttonClicked == false)
        {
            lives--;
            earnedPoint = false;
        }
        update_lives(lives, livesArray, LIVES_SIZE);                    

            // Checkt levels
        if (lives <= 0)
        {
            gameOver = true;
        }
        else
        {
            if (earnedPoint)
            {
                score++;
            }
            
            if (baseDelay > 40)
            {
                baseDelay = baseDelay - delayDecay;
            }
        }
        sevseg_setNumber(score, -1, false);
        buttonClicked = false;
    }
    printf("Score: %d\n", score);
    gameOver = false;
    lives = LIVES_SIZE;
    baseDelay = INITIAL_DELAY;
    score = 0;
    vTaskDelay(100 / portTICK_PERIOD_MS); // Delay zodat het spel niet opnieuw start als je net te laat bent

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
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    // Seven Segment Display Initialiseren
    uint8_t numDigits = 4;
    uint8_t digitPins[] = {39, 38, 1, 2};
    uint8_t segmentPins[] = {37, 36, 35, 0, 45, 48, 47, 21};
    sevseg_begin(numDigits, digitPins, segmentPins);
    sevseg_setChars("8888");

        // Random seed initialiseren
    srand(time(NULL)); // Initaliseer eenmalig de 'seed'
        // Led bar heen en weer
    while (true)
    {
        if (gpio_get_level(BUTTON_PLAY))
        {
            game_loop();
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}