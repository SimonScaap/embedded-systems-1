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

#include "SevSeg.h"
#include "driver/timer.h"

#define TIMER_DIVIDER         	(80)   
#define TIMER_ALARM_VALUE		1000   

#define BLANK_IDX 36 // Must match with 'digitCodeMap'
#define DASH_IDX 37
#define PERIOD_IDX 38
#define ASTERISK_IDX 39
#define UNDERSCORE_IDX 40

static const int32_t powersOf10[] = {
	1, // 10^0
	10,
	100,
	1000,
	10000,
	100000,
	1000000,
	10000000,
	100000000,
	1000000000
}; // 10^9

static const int32_t powersOf16[] = {
	0x1, // 16^0
	0x10,
	0x100,
	0x1000,
	0x10000,
	0x100000,
	0x1000000,
	0x10000000
}; // 16^7

// digitCodeMap indicate which segments must be illuminated to display
// each number.
static const uint8_t digitCodeMap[] = {
	// GFEDCBA  Segments      7-segment map:
	0b00111111, // 0   "0"          AAA
	0b00000110, // 1   "1"         F   B
	0b01011011, // 2   "2"         F   B
	0b01001111, // 3   "3"          GGG
	0b01100110, // 4   "4"         E   C
	0b01101101, // 5   "5"         E   C
	0b01111101, // 6   "6"          DDD
	0b00000111, // 7   "7"
	0b01111111, // 8   "8"
	0b01101111, // 9   "9"
	0b01110111, // 65  'A'
	0b01111100, // 66  'b'
	0b00111001, // 67  'C'
	0b01011110, // 68  'd'
	0b01111001, // 69  'E'
	0b01110001, // 70  'F'
	0b00111101, // 71  'G'
	0b01110110, // 72  'H'
	0b00110000, // 73  'I'
	0b00001110, // 74  'J'
	0b01110110, // 75  'K'  Same as 'H'
	0b00111000, // 76  'L'
	0b00000000, // 77  'M'  NO DISPLAY
	0b01010100, // 78  'n'
	0b00111111, // 79  'O'
	0b01110011, // 80  'P'
	0b01100111, // 81  'q'
	0b01010000, // 82  'r'
	0b01101101, // 83  'S'
	0b01111000, // 84  't'
	0b00111110, // 85  'U'
	0b00111110, // 86  'V'  Same as 'U'
	0b00000000, // 87  'W'  NO DISPLAY
	0b01110110, // 88  'X'  Same as 'H'
	0b01101110, // 89  'y'
	0b01011011, // 90  'Z'  Same as '2'
	0b00000000, // 32  ' '  BLANK
	0b01000000, // 45  '-'  DASH
	0b10000000, // 46  '.'  PERIOD
	0b01100011, // 42 '*'  DEGREE ..
	0b00001000, // 95 '_'  UNDERSCORE
};

// Constant pointers to constant data
const uint8_t * const numeralCodes = digitCodeMap;
const uint8_t * const alphaCodes = digitCodeMap + 10;

int16_t constrain(const int16_t x, const int16_t a, const int16_t b) {
		if(x < a) {
				return a;
		}
		else if(b < x) {
				return b;
		}
		else
				return x;
}

int16_t map(const int16_t au32_IN, const int16_t au32_INmin, const int16_t au32_INmax, const int16_t au32_OUTmin, const int16_t au32_OUTmax) {
		return ((((au32_IN - au32_INmin)*(au32_OUTmax - au32_OUTmin))/(au32_INmax - au32_INmin)) + au32_OUTmin);
}

void IRAM_ATTR timer_group0_isr(void *para){
	portMUX_TYPE timeCriticalMutex = portMUX_INITIALIZER_UNLOCKED;
	portENTER_CRITICAL(&timeCriticalMutex);

	sevseg_refreshDisplay();

	timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);
	timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);

	portEXIT_CRITICAL(&timeCriticalMutex);
}

void sevseg_begin(uint8_t numDigitsIn, const uint8_t digitPinsIn[], const uint8_t segmentPinsIn[]) {

	numDigits = numDigitsIn;
	numSegments = 8; // Ternary 'if' statement
	//Limit the max number of digits to prevent overflowing
	if (numDigits > MAXNUMDIGITS) {
		numDigits = MAXNUMDIGITS;
	}

	// Save the input pin numbers to library variables
	for (uint8_t segmentNum = 0 ; segmentNum < numSegments ; segmentNum++) {
		segmentPins[segmentNum] = segmentPinsIn[segmentNum];
	}

	for (uint8_t digitNum = 0 ; digitNum < numDigits ; digitNum++) {
		digitPins[digitNum] = digitPinsIn[digitNum];
	}

	// Set the pins as outputs, and turn them off
	for (uint8_t digit = 0 ; digit < numDigits ; digit++) {
		gpio_set_direction((gpio_num_t)digitPins[digit], GPIO_MODE_OUTPUT);
		gpio_set_level((gpio_num_t)digitPins[digit], DIGIT_PINS_OFF_VALUE);
	}

	for (uint8_t segmentNum = 0 ; segmentNum < numSegments ; segmentNum++) {
		gpio_set_direction((gpio_num_t)segmentPins[segmentNum], GPIO_MODE_OUTPUT);
		gpio_set_level((gpio_num_t)segmentPins[segmentNum], SEGMENT_PINS_OFF_VALUE);
	}

	sevseg_blank(); // Initialise the display


	timer_config_t config = {
				.divider = TIMER_DIVIDER,
				.counter_dir = TIMER_COUNT_UP,
				.counter_en = TIMER_START,
				.alarm_en = TIMER_ALARM_EN,
				.auto_reload = TIMER_AUTORELOAD_EN,
		}; 
		timer_init(TIMER_GROUP_0, TIMER_0, &config);

		timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);
		timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, TIMER_ALARM_VALUE); 

		timer_enable_intr(TIMER_GROUP_0, TIMER_0);
		timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_group0_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);
}

void sevseg_refreshDisplay() {
	for (uint8_t digitNum = 0 ; digitNum < numDigits ; digitNum++) {
			sevseg_digitOn(digitNum);
			ets_delay_us(40);

			sevseg_digitOff(digitNum);
			ets_delay_us(40);
		}
}

void sevseg_digitOn(uint8_t digitNum) {
		gpio_set_level((gpio_num_t)digitPins[digitNum], DIGIT_PINS_ON_VALUE);

		for (uint8_t segmentNum = 0 ; segmentNum < numSegments ; segmentNum++) {
				if (digitCodes[digitNum] & (1 << segmentNum)) { // Check a single bit
						gpio_set_level((gpio_num_t)segmentPins[segmentNum], SEGMENT_PINS_ON_VALUE);
				}
		}
}

void sevseg_digitOff(uint8_t digitNum) {
		for (uint8_t segmentNum = 0 ; segmentNum < numSegments ; segmentNum++) {;
				gpio_set_level((gpio_num_t)segmentPins[segmentNum], SEGMENT_PINS_OFF_VALUE);
		}
		gpio_set_level((gpio_num_t)digitPins[digitNum], DIGIT_PINS_OFF_VALUE);
}

void sevseg_setNumber(int32_t numToShow, int8_t decPlaces, bool hex) { //int32_t
	sevseg_setNewNum(numToShow, decPlaces, hex);
}

void sevseg_setNumberF(float numToShow, int8_t decPlaces, bool hex) { //float
	int8_t decPlacesPos = constrain(decPlaces, (int8_t)0, (int8_t)MAXNUMDIGITS);
	if (hex) {
		numToShow = numToShow * powersOf16[decPlacesPos];
	}
	else {
		numToShow = numToShow * powersOf10[decPlacesPos];
	}
	// Modify the number so that it is rounded to an integer correctly
	numToShow += (numToShow >= 0.f) ? 0.5f : -0.5f;
	sevseg_setNewNum((int32_t)numToShow, (int8_t)decPlaces, hex);
}

void sevseg_setNewNum(int32_t numToShow, int8_t decPlaces, bool hex) {
	uint8_t digits[MAXNUMDIGITS];
	sevseg_findDigits(numToShow, decPlaces, hex, digits);
	sevseg_setDigitCodes(digits, decPlaces);
}

void sevseg_setSegments(const uint8_t segs[]) {
	for (uint8_t digit = 0; digit < numDigits; digit++) {
		digitCodes[digit] = segs[digit];
	}
}

void sevseg_setSegmentsDigit(const uint8_t digitNum, const uint8_t segs) {
	if (digitNum < numDigits) {
		digitCodes[digitNum] = segs;
	}
}

void sevseg_getSegments(uint8_t segs[]) {
	for (uint8_t digit = 0; digit < numDigits; digit++) {
		segs[digit] = digitCodes[digit];
	}
}

void sevseg_setChars(const char str[]) {
	for (uint8_t digit = 0; digit < numDigits; digit++) {
		digitCodes[digit] = 0;
	}

	uint8_t strIdx = 0; // Current position within str[]
	for (uint8_t digitNum = 0; digitNum < numDigits; digitNum++) {
		char ch = str[strIdx];
		if (ch == '\0') break; // NULL string terminator
		if (ch >= '0' && ch <= '9') { // Numerical
			digitCodes[digitNum] = numeralCodes[ch - '0'];
		}
		else if (ch >= 'A' && ch <= 'Z') {
			digitCodes[digitNum] = alphaCodes[ch - 'A'];
		}
		else if (ch >= 'a' && ch <= 'z') {
			digitCodes[digitNum] = alphaCodes[ch - 'a'];
		}
		else if (ch == ' ') {
			digitCodes[digitNum] = digitCodeMap[BLANK_IDX];
		}
		else if (ch == '.') {
			digitCodes[digitNum] = digitCodeMap[PERIOD_IDX];
		}
		else if (ch == '*') {
			digitCodes[digitNum] = digitCodeMap[ASTERISK_IDX];
		}
		else if (ch == '_') {
			digitCodes[digitNum] = digitCodeMap[UNDERSCORE_IDX];
		}
		else {
			// Every unknown character is shown as a dash
			digitCodes[digitNum] = digitCodeMap[DASH_IDX];
		}

		strIdx++;
		// Peek at next character. If it's a period, add it to this digit
		if (str[strIdx] == '.') {
			digitCodes[digitNum] |= digitCodeMap[PERIOD_IDX];
			strIdx++;
		}
	}
}

void sevseg_blank(void) {
	for (uint8_t digitNum = 0 ; digitNum < numDigits ; digitNum++) {
		digitCodes[digitNum] = digitCodeMap[BLANK_IDX];
	}
	sevseg_digitOff(0);
}

void sevseg_findDigits(int32_t numToShow, int8_t decPlaces, bool hex, uint8_t digits[]) {
	const int32_t * powersOfBase = hex ? powersOf16 : powersOf10;
	const int32_t maxNum = powersOfBase[numDigits] - 1;
	const int32_t minNum = -(powersOfBase[numDigits - 1] - 1);

	// If the number is out of range, just display dashes
	if (numToShow > maxNum || numToShow < minNum) {
		for (uint8_t digitNum = 0 ; digitNum < numDigits ; digitNum++) {
			digits[digitNum] = DASH_IDX;
		}
	}
	else {
		uint8_t digitNum = 0;

		// Convert all number to positive values
		if (numToShow < 0) {
			digits[0] = DASH_IDX;
			digitNum = 1; // Skip the first iteration
			numToShow = -numToShow;
		}

		// Find all digits for base's representation, starting with the most
		// significant digit
		for ( ; digitNum < numDigits ; digitNum++) {
			int32_t factor = powersOfBase[numDigits - 1 - digitNum];
			digits[digitNum] = numToShow / factor;
			numToShow -= digits[digitNum] * factor;
		}

		// Find unnnecessary leading zeros and set them to BLANK
		if (decPlaces < 0) decPlaces = 0;
		/*if (!leadingZeros) {
			for (digitNum = 0 ; digitNum < (numDigits - 1 - decPlaces) ; digitNum++) {
				if (digits[digitNum] == 0) {
					digits[digitNum] = BLANK_IDX;
				}
				// Exit once the first non-zero number is encountered
				else if (digits[digitNum] <= 9) {
					break;
				}
			}
		}*/

	}
}

void sevseg_setDigitCodes(const uint8_t digits[], int8_t decPlaces) {

	// Set the digitCode for each digit in the display
	for (uint8_t digitNum = 0 ; digitNum < numDigits ; digitNum++) {
		digitCodes[digitNum] = digitCodeMap[digits[digitNum]];
		// Set the decimal point segment
		if (decPlaces >= 0) {
			if (digitNum == numDigits - 1 - decPlaces) {
				digitCodes[digitNum] |= digitCodeMap[PERIOD_IDX];
			}
		}
	}
}

/// END ///