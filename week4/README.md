# Week 4

## Vraagstukken en theorie

### Wat is een minterm?

### ADC volgens parallelmethode

### ADC volgens weegmethode

## Flappy Bird

### Een korte beschrijving van het project Flappybird

In dit project gebruik je een microcontroller om Flappybird online op de DLO te spelen. Hiervoor gebruik je een push-button aan. Wanneer je op de knop drukt moet de ESP het ascii-karakter ">". De ESP moet communiceren op een baudrate van 115200.

### Hoe werkt het project Flappybird

[Het programma](./flappy-bird/src/main.c) is heel erg simpel. Het mechanisme om het vogeltje te laten sprinten is namelijk maar 1 functie: `printf(">\n")`. Bij deze functie wordt UART onder de motorkap geregeld. Het programma checkt voor input met `gpio_get_level()` en slaat dat op in een variable `buttonValue`. Daarna wordt `buttonValue` vergeleken met `buttonValueCallback` om te kijken of de staat van de knop veranderd is. Dan wordt het ascii teken verstuurd.

## Verkeerslicht Improved

### Een korte beschrijving van het project Verkeerslicht Improved

In dit project gebruik je een microcontroller om een stoplicht op afstand te besturen. Hiervoor gebruik je UART. Om de commando's te sturen gebruik je het programma PuTTY.

### Hoe werkt het?

[Het programma](./verkeerslicht-improved/src/main.c) werkt door eerst de uart interface te configureren. Om ons opweg te helpen heeft onze docent het volgende stukje code geschreven:

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define BUF_SIZE (1024)

void app_main() {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(UART_NUM_0, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);

        if (len) {
            data[len] = '\0';
            printf("> %s\n", data);
        }
    }
}
```

 Je laptop en ESP communiceren standaard over `UART_NUM_0`. Door `uart_config` aan te maken kun je de baudrate, hoeveelheid databits, en hoeveelheid stopbits meegeven zodat de laptop en ESP op de juiste manier kunnen communiceren. Door in de functie `uart_set_pin()` `UART_PIN_NO_CHANGE` mee te geven als pins, kun je zeggen dat de ESP de al-geconfigureerde UART-pins moet gebruiken. Daarna wordt een databuffer aangemaakt met `malloc()`. `malloc()` geeft een pointer terug die gebruikt kan worden om naar het adres va de databuffer te schrijven. Met task kun je met de functie `xTaskCreate()` een "thread" aanmaken. Dit proces draait dan parallel met `app_main()`. Dit is handig, omdat je zo voor input kan blijven checken terwijl op de achtergrond iets anders gedaan wordt. Met de functie `uart_read_bytes()` wordt de data in de RX buffer van de ESP uitgelezen en vervolgens in een switch-case verwerkt. Bij commando's '1', '2' en '3' wordt de functie `set_one_to_high()` gebruikt. Deze functie zet uit een array met leds elke led uit behalve 1. De task die de oranje led laat knipperen wordt ook gepauzeerd met `vTasksuspend()`. Bij commando 'A' worden de rode en groene led uitgezet, en gaat de task weer verder met `vTaskResume()`.

### Schematisch en Fysiek

### Bronvermelding en Leermateriaal

- [ESP-IDF UART documentatie](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html)
- [DLO voorbeeld data ontvangen](https://dlo.mijnhva.nl/d2l/le/content/467515/Home)
- [FreeRTOS Task docs](https://www.freertos.org/a00125.html)
- [Youtube tutorial task creation](https://www.youtube.com/watch?v=nYlpeApGXwQ&t=187s)
- [ESP-IDF Task docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/system/freertos.html?highlight=xtaskcreate#_CPPv411xTaskCreate14TaskFunction_tPCKcK22configSTACK_DEPTH_TYPEPCv11UBaseType_tPC12TaskHandle_t)
- [Artikel over malloc functie](https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/)
