#include <stdio.h>
#include <string.h>

int main() {
    char sentence[] = {'h', 'a', 'l', 'l', 'o', '\0'};
    int stringLength = (int)strlen(sentence);
    char sentenceNew[stringLength];

    // Print de originele string
    printf("sentence: %s\n", sentence);
    //printf("sentence length: %d\n", stringLength);

    //
    for (int i = 0; i < stringLength; ++i) {
        sentenceNew[i] = sentence[stringLength - (i + 1)];
        //printf("index: %d  char: %c\n", i, sentenceNew[i]);

    }

    printf("sentence reversed: %s\n", sentenceNew);
    return 0;
}