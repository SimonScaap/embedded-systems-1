#include <stdio.h>

/**
 * @brief Check bit value on a certain position
 * @param position Position of the bit starting at 0
 * @param number The number checked
 * @return
 * - 0 if the bit on the position is 0\return
 * - 1 if the bit on the position is 1
 */
int bit_check(int position, int number) {
    // Bit shift positie
    if (((1<<position) & number) != 0) {
        return 1;
    } else {
        return 0;
    }
}

/**
 *
 * @param position The position of the but you want to set
 * @param value 0 or 1, The value you want to changa a but to
 * @param number The number the bit should be set .
 * @return
 */
int bit_set(int position, int value, int number) {
    // als bit van position hetzelfde is als bit van value dan zelfde
    // als bit van position anders is als bit van value dan anders
    if (bit_check(position, number) == value) {
        return number;
    } else {
        return number ^ (1 << position);
    }

}

void switcheroo(int x, int y) {
    printf("x = %d | y = %d\n", x, y);

    x = x ^ y;
    y = y ^ x;
    x = x ^ y;
    printf("x = %d | y = %d\n", x, y);
    printf("\n");
}

int main() {
    printf("Opdracht 1:\n");
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            switcheroo(i, j);
        }
    }

    printf("\nOpdracht 2:\n");


    return 0;
}