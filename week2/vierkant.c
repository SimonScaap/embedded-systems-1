#include <stdio.h>

int width = 15;
int height = 7;

int main() {
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
