#include <stdio.h>

int main() {
    printf("Mario!!\n");
    int grid_size = 5;
    for (int y = 0; y < grid_size; ++y) {

        for (int x = 0; x < grid_size; ++x) {
            if((grid_size - x) <= y + 1) {
                printf("# ");
            } else {
                printf("  ");
            };
        }
        printf("\n");

    }
    return 0;
}