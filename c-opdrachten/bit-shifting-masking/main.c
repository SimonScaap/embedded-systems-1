#include <stdio.h>

int bit_check(int position, int number) {
    // Bit shift positie
    if (((1<<position) & number) != 0) {
        return 1;
    } else {
        return 0;
    }
}

int bit_set(int position, int value, int number) {
    // als bit van position hetzelfde is als bit van value dan zelfde
    // als bit van position anders is als bit van value dan anders
    
}

int main() {
    int number = 7;

    printf("value: %d\n", number);
    printf("value: %d\n", bit_check(0, number));
    printf("value: %d\n", bit_check(3, number));
    printf("value: %d\n", bit_set(0, 1, number));
    printf("value: %d\n", bit_set(0, 0, number));
    printf("value: %d\n", bit_set(4, 1, number));
}

