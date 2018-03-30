#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void insert(int fScore, int *number);

void printAll(int *number, double timeComplexity);

void MinHeapDecreaseKey(int *number, int x, int key);

void MinHeapify(int *number, int i);

void BuildMinHeap(int *number);

int HeapExtractMin(int *number);

void MinHeapInsert(int *number, int key);

void MinHeapsort(int *number);

void MinHeapDecreaseKeyNoConstraint(int *number, int x, int key);

int Parent(int i);

int Left(int i);

int Right(int i);

void swap(int *number, int a, int b);

int heapTimeComplexity;

int heapSize;

int size;

int main() {
    FILE *file;
    char *filename = "test.txt";
    int fScore;
    double timeComplexity;
    time_t startTime = 0, endTime = 0;
    int *number = malloc(sizeof(int));
    int minValue = 0;
    size = 0;
    heapSize = 0;
    heapTimeComplexity = 0;

    if ((file = fopen(filename, "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }

    while (feof(file) == 0) {
        fscanf(file, "%d", &fScore);
        number = realloc(number, (size + 1) * sizeof(int));
        insert(fScore, number);
        size++;
    }
    heapSize = size;
    fclose(file);

    printf("시작\n");

    startTime = clock();
    BuildMinHeap(number);
    printAll(number, timeComplexity);
    MinHeapsort(number);
    printAll(number, timeComplexity);
    minValue = HeapExtractMin(number);
    printf("minValue : %d\n", minValue);
    printAll(number, timeComplexity);
    MinHeapInsert(number, 99);
    printAll(number, timeComplexity);
    MinHeapDecreaseKey(number, 3, 1);
    printAll(number, timeComplexity);
    MinHeapDecreaseKeyNoConstraint(number, 0, 8);
    printAll(number, timeComplexity);
    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printf("실행 시간 : %f(ms) \theap 횟수 : %d \n", timeComplexity, heapTimeComplexity);


    printf("완료\n");
    free(number);
    return 1;
}

void insert(int fScore, int *number) {
    number[size] = fScore;
}

void printAll(int *number, double timeComplexity) {
    int i;
    for (i = 0; i < heapSize; i++) {
        printf("%d \t", number[i]);
    }
    printf("\nheap 횟수 : %d\n", heapTimeComplexity);
}

void MinHeapsort(int *number) {
    int i;
    BuildMinHeap(number);
    for (i = size - 1; i > 0; i--) {
        swap(number, 0, i);
        size--;
        MinHeapify(number, 0);
    }
    size = heapSize;
}

void BuildMinHeap(int *number) {
    int i;
    size = heapSize;
    for (i = Parent(size - 1); i >= 0; i--) {
        MinHeapify(number, i);
    }
}

void MinHeapify(int *number, int i) {
    int lc;
    int rc;
    int smallest;
    int sc;
    heapTimeComplexity++;
    lc = Left(i);
    rc = Right(i);
    if(lc < size){
        if(rc >= size){
            if(number[i] > number[lc]){
                smallest = lc;
                swap(number, i, lc);
                MinHeapify(number, smallest);
            }
        }else{
            sc = (number[lc] < number[rc]) ? lc : rc;
            if(number[i] > number[sc]){
                smallest = sc;
                swap(number, i, sc);
                MinHeapify(number, smallest);
            }
        }
    }
}

int HeapExtractMin(int *number) {
    int i;
    int min_value;
    BuildMinHeap(number);
    i = size - 1;
    min_value = number[0];
    swap(number, 0, i);
    size--;
    heapSize = size;
    number = realloc(number, size * sizeof(int));
    MinHeapify(number, 0);
    return min_value;
}

void MinHeapInsert(int *number, int key) {
    heapSize++;
    size++;
    number = realloc(number, heapSize * sizeof(int));
    number[heapSize - 1] = 99999;
    MinHeapDecreaseKey(number, heapSize - 1, key);
}

void MinHeapDecreaseKey(int *number, int x, int key) {
    if (key > number[x]) {
        printf("new key is bigger than current key");
    } else {
        number[x] = key;
        while ((x > 0) && (number[Parent(x)] > number[x])) {
            swap(number, x, Parent(x));
            x = Parent(x);
        }
    }
}

void MinHeapDecreaseKeyNoConstraint(int *number, int x, int key) {
    if (key > number[x]) {
        number[x] = key;
        MinHeapify(number, x);
    }  else {
        number[x] = key;
        while ((x > 0) && (number[Parent(x)] > number[x])) {
            swap(number, x, Parent(x));
            x = Parent(x);
        }
    }
}

int Parent(int i) {
    return floor((i - 1) / 2);
}

int Left(int i) {
    return floor((i + 1) * 2) - 1;
}

int Right(int i) {
    return floor((i + 1) * 2);
}

void swap(int *number, int a, int b) {
    int i;
    i = number[a];
    number[a] = number[b];
    number[b] = i;
}