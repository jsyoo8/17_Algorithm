#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insert(int fScore, int *number, int size);

void bubbleSortScore(int *number, int size);

void printAll(int *number, int size, double timeComplexity);

int *sorted;

int bubbleTimeComplexity;

int main() {
    FILE *file;
    //char *filename = argv[1];
    char *filename = "test_10000.txt";
    int fScore;
    int size = 0;
    double timeComplexity;
    time_t startTime = 0, endTime = 0;
    int *number = malloc(sizeof(int));
    bubbleTimeComplexity = 0;
/*
    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }
*/
    if ((file = fopen(filename, "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }

    while (feof(file) == 0) {
        fscanf(file, "%d", &fScore);
        number = realloc(number, (size + 1) * sizeof(int));
        insert(fScore, number, size);
        size++;
    }
    fclose(file);

    sorted = malloc(size * (sizeof(int)));

    printf("시작\n");

    startTime = clock();

    bubbleSortScore(number, size);

    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printAll(number, size, timeComplexity);
    printf("완료\n");
    free(number);
    free(sorted);
    return 0;
}

void insert(int fScore, int *number, int size) {
    number[size] = fScore;
}


void bubbleSortScore(int *number, int size) {
    int bubbleNode, curr, score;
    for (bubbleNode = 0; bubbleNode < size - 1; bubbleNode++) {
        for (curr = 0; curr < size - 1 - bubbleNode; curr++) {
            bubbleTimeComplexity++;
            if (number[curr] > number[curr + 1]) {
                score = number[curr];
                number[curr] = number[curr + 1];
                number[curr + 1] = score;
            }
        }
    }
}

void printAll(int *number, int size, double timeComplexity) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%d \n", number[i]);
    }
    printf("실행 시간 : %f(ms) \nbubble 비교 횟수 : %d \n", timeComplexity, bubbleTimeComplexity);
}