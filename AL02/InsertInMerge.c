#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

void insert(int fScore, int size);

void sortScore(int size, int n);

void printAll(int size, double timeComplexity);

void sort(int left, int right, int n);

void mergeSort(int left, int mid, int right);

void insertSort(int left, int right);

int *number;

int *sorted;

int mergeTimeComplexity;

int insertTimeComplexity;

int main() {
    FILE *file;
    //char *filename = argv[1];
    char *filename = "test_10000.txt";
    int fScore;
    int size = 0;
    double timeComplexity;
    time_t startTime = 0, endTime = 0;
    number = malloc(sizeof(int));
    mergeTimeComplexity = 0;
    insertTimeComplexity = 0;
    int n = 4;
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
        insert(fScore, size);
        size++;
    }
    fclose(file);

    sorted = malloc(size * (sizeof(int)));
    printf("시작\n");

    startTime = clock();
    sortScore(size, n);
    endTime = clock();
    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);
    timeComplexity *= 1000.0;

    printAll(size, timeComplexity);
    printf("완료\n");
    free(number);
    free(sorted);
    return 0;
}

void insert(int fScore, int size) {
    number[size] = fScore;
}

void sortScore(int size, int n) {
    sort(0, size, n);
}

void sort(int left, int right, int n) {
    int mid;
    if ((right - left) > 1) {
        mid = (left + right) / 2;
        sort(left, mid, n);
        sort(mid, right, n);
        if ((right - left) > n) {
            mergeSort(left, mid, right);
        } else {
            insertSort(left, right);
        }
    }
}

void mergeSort(int left, int mid, int right) {
    int i, j, k;
    i = left;
    j = mid;

    mergeTimeComplexity++;

    for (k = left; (i < mid && j < right); k++) {
        if (number[i] < number[j]) {
            sorted[k] = number[i++];
        } else {
            sorted[k] = number[j++];
        }
    }
    if (i == mid) {
        memcpy((sorted + k), (number + j), (right - j) * sizeof(int));
    } else {
        memcpy((sorted + k), (number + i), (mid - i) * sizeof(int));
    }
    //printf("디버깅 left=%d mid=%d right=%d \n", left, mid, right);
    memcpy((number + left), (sorted + left), (right - left) * sizeof(int));
}

void insertSort(int left, int right) {
    int i, j;
    int insertNode, curr, score;

    insertTimeComplexity++;

    for (i = left + 1; i < right; i++) {
        insertNode = i;
        curr = left;
        while (curr != insertNode) {
            if (number[curr] > number[insertNode]) {
                score = number[insertNode];
                for (j = 0; j < (insertNode - curr); j++) {
                    number[insertNode - j] = number[insertNode - j - 1];
                }
                number[curr] = score;
                break;
            }
            curr++;
        }
    }
}

void printAll(int size, double timeComplexity) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%d \n", number[i]);
    }
    printf("실행 시간 : %f(ms) \nmerge 실행 횟수 : %d \ninsert 실행 횟수 : %d \n", timeComplexity, mergeTimeComplexity,
           insertTimeComplexity);
}