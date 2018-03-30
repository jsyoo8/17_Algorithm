#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

void insert(int fScore, int *number, int size);

void mergeSortScore(int *number, int size);

void printAll(int *number, int size, double timeComplexity);

void merge(int *number, int left, int mid, int right);

int *sorted;

int mergeTimeComplexity;

int main() {
    FILE *file;
    //char *filename = argv[1];
    char *filename = "test_10000.txt";
    int fScore;
    int size = 0;
    double timeComplexity;
    time_t startTime = 0, endTime = 0;
    int *number = malloc(sizeof(int));
    mergeTimeComplexity = 0;
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

    mergeSortScore(number, size);

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

void mergeSortScore(int *number, int size) {

    int arrayScale;
    int left;

    for (arrayScale = 1; arrayScale < size; arrayScale = 2 * arrayScale) {
        for (left = 0; left < size - 1; left += 2 * arrayScale) {
            int mid = left + arrayScale;
            int right = mid + arrayScale;
            if (right <= size) {
                merge(number, left, mid, right);
            } else {
                merge(number, left, mid, size);
            }
        }
    }



    /*
    double logNum = floor(log(size - 1) / log(2));
    int midSize = 0;
    int arrayScale = pow(2, logNum + 1);
    int firstArrayScale = arrayScale;
    int *midArray = malloc(sizeof(int));
    int left, right, i;

    while (arrayScale >= 2) {
        arrayScale /= 2;
        printf("arrayScale=%d\n", arrayScale);
        midArray = realloc(midArray, (midSize + 1) * sizeof(int));
        midArray[midSize] = 0;
        midSize++;
        for (i = arrayScale; i < size; i = i + arrayScale) {
            midArray = realloc(midArray, (midSize + 1) * sizeof(int));
            midArray[midSize] = i;
            midSize++;
        }
    }

    while (arrayScale < firstArrayScale) {
        printf("arrayScale=%d\n", arrayScale);
        while (midArray[midSize - 1] != 0) {
            midSize--;
            left = midArray[midSize] - arrayScale;
            right = midArray[midSize] + arrayScale;
            if (right <= size) {
                merge(number, left, midArray[midSize], right);
            }else{
                merge(number, left, midArray[midSize], size);
            }
        }
        midSize--;
        arrayScale *= 2;
    }
    free(midArray);
     */
}

void merge(int *number, int left, int mid, int right) {
    int i, j, k;
    //int l;
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
        /*
        for (l = j; l < right; l++) {
            sorted[k++] = number[l];
        }
         */
    } else {
        memcpy((sorted + k), (number + i), (mid - i) * sizeof(int));
        /*
        for (l = i; l < mid; l++) {
            sorted[k++] = number[l];
        }
        */
    }
    //printf("디버깅 left=%d mid=%d right=%d \n", left, mid, right);
    memcpy((number + left), (sorted + left), (right - left) * sizeof(int));
}

void printAll(int *number, int size, double timeComplexity) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%d \n", number[i]);
    }
    printf("실행 시간 : %f(ms), merge 실행 횟수 : %d \n", timeComplexity, mergeTimeComplexity);
}