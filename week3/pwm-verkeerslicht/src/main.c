/*
Dit programma zorgt ervoor dat een led aan en uit fade door middel van pwm
@author Simon Zweers
@date 04 12 2022
*/
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <esp_err.h>
#include <math.h> 

#define HIGH 1
#define LOW 0

#define GREEN 6
#define YELLOW 5
#define RED 4
#define BUTTON_INPUT 18

// Delays voor stoplichtcycle
#define DELAY1_MS 1000
#define TRAFFIC_DELAY_MS 3000

// LEDC Config Values
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO YELLOW
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT
#define LED_DUTY 4095 // 
#define LEDC_FREQUENCY 5000

#define ARRAY_LENGTH 3
int LED_PINS_ARR[3] = {GREEN, YELLOW, RED};

void set_one_to_high(int pinToSet, int ledPinsArray[], int arrayLength) {
    for (size_t i = 0; i < arrayLength; i++)
    {
        if (pinToSet == ledPinsArray[i])
        {
            gpio_set_level(ledPinsArray[i], HIGH);
            printf("Set %d to HIGH\t", ledPinsArray[i]);
        } 
        else
        {
            gpio_set_level(ledPinsArray[i], LOW);
            printf("Set %d to LOW\t", ledPinsArray[i]);
        }   
    }
    printf("\n");
}

void traffic_light_cycle() {
    // GREEN
    vTaskDelay(TRAFFIC_DELAY_MS / portTICK_PERIOD_MS);
    set_one_to_high(GREEN, LED_PINS_ARR, ARRAY_LENGTH);
    printf("turned on GREEN\n");
    vTaskDelay(TRAFFIC_DELAY_MS / portTICK_PERIOD_MS);
    // YELLOW
    set_one_to_high(YELLOW, LED_PINS_ARR, ARRAY_LENGTH);
    printf("turned on YELLOW\n");
    vTaskDelay(TRAFFIC_DELAY_MS / portTICK_PERIOD_MS);
    // RED
    set_one_to_high(RED, LED_PINS_ARR, ARRAY_LENGTH);
    printf("turned on RED\n");
}

static void ledc_init() {
    // LEDC timer config
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .duty_resolution = LEDC_DUTY_RES,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // LEDC chanmel config
    ledc_channel_config_t ledc_channel = {
        .channel = LEDC_CHANNEL,
        .duty = 0,
        .gpio_num = LEDC_OUTPUT_IO,
        .hpoint = 0,
        .intr_type = LEDC_INTR_DISABLE,
        .speed_mode = LEDC_MODE,
        .timer_sel = LEDC_TIMER
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}
/*
int duty_calculation(int percentage) {
    return (int)((pow(2, 13) - 1) * (percentage / 100));
}
*/

void app_main() {
    printf("Setting GPIO / ledc PWM\n");
    ledc_init();

    gpio_set_direction(GREEN, GPIO_MODE_OUTPUT);
    //gpio_set_direction(YELLOW, GPIO_MODE_OUTPUT);
    gpio_set_direction(RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_INPUT, GPIO_MODE_INPUT);
    ledc_fade_func_install(0);
    /*
    int buttonValue = 0;
    int buttonValueCallback = 0;
    int tempCount = 0;
    
    */
    int minDuty = 0;
    int maxDuty = 8191;
    while (true)
    {
        // set_one_to_high(RED, LED_PINS_ARR, ARRAY_LENGTH);
        // while (!breakLoop)
        // {
            
        //     buttonValue = gpio_get_level(BUTTON_INPUT);
        //     if (buttonValue > buttonValueCallback)
        //     {
        //         tempCount += 1;
        //         printf("Button Pressed! : %d\n", tempCount);
        //         breakLoop = true;
        //     }
        //     buttonValueCallback = buttonValue;
            
        //     vTaskDelay(10/ portTICK_PERIOD_MS);
            
        // }
        ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL, maxDuty, 500);
        printf("Set fade to maxDuty\n");
        ledc_fade_start(LEDC_MODE, LEDC_CHANNEL, LEDC_FADE_WAIT_DONE );
        printf("Start fade to maxDuty\n");

        ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL, minDuty, 500);
        printf("Set fade to minDuty\n");
        ledc_fade_start(LEDC_MODE, LEDC_CHANNEL, LEDC_FADE_WAIT_DONE );
        printf("Start fade to minDuty\n");
    }
}