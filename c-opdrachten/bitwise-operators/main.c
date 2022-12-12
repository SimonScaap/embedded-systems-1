#include <stdio.h>

int main() {
    printf("Opdracht 1:\n");
    int x = 10;
    int y = 5;
    printf("x = %d | y= %d\n", x, y);
    int mask = x ^ y;
    x = x ^ mask;
    y = y ^ mask;
    printf("x = %d | y = %d\n", x, y);

    printf("\nOpdracht 2:\n");
    int a = 13;
    int i = 0;

    printf("a = %d\n", a);
    while ((a & (1 << i)) != 0) {
        printf("%d \n", ((a & (1 << i)) != 0));
        printf("%d\n", a);
        a = a | (1 << i);
        i++;
    }
    printf("a = %d\n", a);
    return 0;
}

/*
 * 1010
 * 0101
 */