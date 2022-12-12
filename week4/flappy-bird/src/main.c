#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/uart.h"

#define BUTTON_INPUT 4
#define COMMAND '>'

void uart_config() {
    const uart_port_t uart_num = UART_NUM_2;
    uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
    .rx_flow_ctrl_thresh = 122,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
}

void app_main() {
    gpio_set_direction(BUTTON_INPUT, GPIO_MODE_INPUT);
    // Setup UART buffered IO with event queue
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));
    printf(">\n");
    int buttonValue = 0;
    int buttonValueCallback = 0;
    unsigned char command = '>';

    while (true)
    {
        buttonValue = gpio_get_level(BUTTON_INPUT);
        if (buttonValue > buttonValueCallback)
        {
            printf(">\n");        
        }
        buttonValueCallback = buttonValue;
        vTaskDelay(20/portTICK_PERIOD_MS);
    }
}