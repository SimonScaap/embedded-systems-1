#include <stdio.h>

int bit_check(int position, int number) {
    // TODO: Maak af
    return 0;
}

int bit_set(int position, int value, int number) {
    // TODO: Maak af
    return 0;
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

