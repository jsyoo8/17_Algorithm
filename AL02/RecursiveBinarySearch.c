#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insert(int fScore, int *number, int size);

void printAll(int *number, double timeComplexity, int index);

int search(int *number, int size, int searchNumber);

int binarySearch(int *number, int left, int right, int searchNumber);

int *sorted;

int binarySearchTimeComplexity;

int main() {
    FILE *file;
    //char *filename = argv[1];
    char *filename = "test_recursive.txt";
    int fScore;
    int size = 0;
    double timeComplexity;
    int searchNumber;
    time_t startTime = 0, endTime = 0;
    int *number = malloc(sizeof(int));
    binarySearchTimeComplexity = 0;
    int index = -1;
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

    searchNumber = 1050;

    sorted = malloc(size * (sizeof(int)));

    printf("시작\n");

    startTime = clock();

    index = search(number, size, searchNumber);

    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printAll(number, timeComplexity, index);

    printf("완료\n");
    free(number);
    free(sorted);
    return 0;
}

void insert(int fScore, int *number, int size) {
    number[size] = fScore;
}

int search(int *number, int size, int searchNumber) {
    return binarySearch(number, 0, size, searchNumber);
}

int binarySearch(int *number, int left, int right, int searchNumber) {
    int mid = ((right - left) / 2) + left;
    binarySearchTimeComplexity++;
    if ((right - left) == 0) {
        return -1;
    }
    if (number[mid] > searchNumber) {
        return binarySearch(number, left, mid, searchNumber);
    } else if (number[mid] < searchNumber) {
        return binarySearch(number, mid, right, searchNumber);
    } else {
        return mid;
    }
}

void printAll(int *number, double timeComplexity, int index) {
    if (index < 0) {
        printf("찾으시는 숫자는 존재하지 않습니다.");
    } else {
        printf("찾는 숫자 : %d \n위치 : %d \n실행 시간 : %f(ms), 탐색 횟수 : %d \n", number[index], index, timeComplexity,
               binarySearchTimeComplexity);
    }
}