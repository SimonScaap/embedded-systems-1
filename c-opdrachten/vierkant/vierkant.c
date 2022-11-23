#include "stdio.h"
//#include "stdlib.h"

int main() {
    int width = 6;
    int height= 10;

    printf("width: %d\n", width);
    printf("height: %d\n", height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == 0 || i == (height - 1) || j == 0 || j == (width - 1)) {
                printf("# ");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
    return 0;
}
