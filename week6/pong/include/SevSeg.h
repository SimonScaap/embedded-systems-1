/* SevSeg Library
 *
 * Copyright 2020 Dean Reading
 *
 * This library allows an Arduino to easily display numbers and letters on a
 * 7-segment display without a separate 7-segment display controller.
 *
 * See the included readme for instructions.
 * https://github.com/DeanIsMe/SevSeg
 * 
 * Updated by Mats Otten, 2022, to work with the ESP32 ESP-IDF.
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <rom/ets_sys.h>

#define HIGH 1
#define LOW 0

#define SEGMENT_PINS_OFF_VALUE LOW
#define SEGMENT_PINS_ON_VALUE HIGH

#define DIGIT_PINS_OFF_VALUE HIGH
#define DIGIT_PINS_ON_VALUE LOW

#ifndef MAXNUMDIGITS
    #define MAXNUMDIGITS 8 // Can be increased, but the max number is 2^31
#endif

#ifndef SevSeg_h
    #define SevSeg_h

    uint8_t digitPins[MAXNUMDIGITS];
    uint8_t segmentPins[8];
    uint8_t numDigits;
    uint8_t numSegments;
    uint8_t digitCodes[MAXNUMDIGITS]; // The active setting of each segment of each digit

    void sevseg_refreshDisplay();
    void sevseg_begin(uint8_t numDigitsIn, const uint8_t digitPinsIn[], const uint8_t segmentPinsIn[]);
    void sevseg_setNumber(int32_t numToShow, int8_t decPlaces, bool hex);
    void sevseg_setNumberF(float numToShow, int8_t decPlaces, bool hex);
    void sevseg_setSegments(const uint8_t segs[]);
    void sevseg_getSegments(uint8_t segs[]);
    void sevseg_setSegmentsDigit(const uint8_t digitNum, const uint8_t segs);
    void sevseg_setChars(const char str[]);
    void sevseg_blank(void);

    void sevseg_setNewNum(int32_t numToShow, int8_t decPlaces, bool hex);
    void sevseg_findDigits(int32_t numToShow, int8_t decPlaces, bool hex, uint8_t digits[]);
    void sevseg_setDigitCodes(const uint8_t nums[], int8_t decPlaces);
    void sevseg_digitOn(uint8_t digitNum);
    void sevseg_digitOff(uint8_t digitNum);

#endif //SevSeg_h
/// END ///