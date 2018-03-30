#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void insert(int fScore, int *number);

void printAll(int *number, double timeComplexity);

void MaxHeapIncreaseKey(int *number, int x, int key);

void MaxHeapify(int *number, int i);

void BuildMaxHeap(int *number);

int HeapExtractMax(int *number);

void MaxHeapInsert(int *number, int key);

void MaxHeapsort(int *number);

int Parent(int i);

int Left(int i);

int Right(int i);

void swap(int *number, int a, int b);

void MaxHeapIncreaseKeyNoConstraint(int *number, int x, int key);

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
    int maxValue = 0;
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
    BuildMaxHeap(number);
    printAll(number, timeComplexity);
    MaxHeapsort(number);
    printAll(number, timeComplexity);
    maxValue = HeapExtractMax(number);
    printf("maxValue : %d\n", maxValue);
    printAll(number, timeComplexity);
    MaxHeapInsert(number, 99);
    printAll(number, timeComplexity);
    MaxHeapIncreaseKey(number, 3, 66);
    printAll(number, timeComplexity);
    MaxHeapIncreaseKeyNoConstraint(number, 0, 70);
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

void MaxHeapsort(int *number) {
    int i;
    BuildMaxHeap(number);
    for (i = size - 1; i > 0; i--) {
        swap(number, 0, i);
        size--;
        MaxHeapify(number, 0);
    }
    size = heapSize;
}

void BuildMaxHeap(int *number) {
    int i;
    size = heapSize;
    for (i = Parent(size - 1); i >= 0; i--) {
        MaxHeapify(number, i);
    }
}

void MaxHeapify(int *number, int i) {
    int lc;
    int rc;
    int largest;
    int bc;
    heapTimeComplexity++;
    lc = Left(i);
    rc = Right(i);
    if(lc < size){
        if(rc >= size){
            if(number[i] < number[lc]){
                largest = lc;
                swap(number, i, lc);
                MaxHeapify(number, largest);
            }
        }else{
            bc = (number[lc] > number[rc]) ? lc : rc;
            if(number[i] < number[bc]){
                largest = bc;
                swap(number, i, bc);
                MaxHeapify(number,largest);
            }
        }
    }
}

int HeapExtractMax(int *number) {
    int i;
    int max_value;
    BuildMaxHeap(number);
    i = size - 1;
    max_value = number[0];
    swap(number, 0, i);
    size--;
    heapSize = size;
    number = realloc(number, size * sizeof(int));
    MaxHeapify(number, 0);
    return max_value;
}

void MaxHeapInsert(int *number, int key) {
    heapSize++;
    size++;
    number = realloc(number, heapSize * sizeof(int));
    number[heapSize - 1] = -1;
    MaxHeapIncreaseKey(number, heapSize - 1, key);
}

void MaxHeapIncreaseKey(int *number, int x, int key) {
    if (key < number[x]) {
        printf("new key is smaller than current key");
    } else {
        number[x] = key;
        while ((x > 0) && (number[Parent(x)] < number[x])) {
            swap(number, x, Parent(x));
            x = Parent(x);
        }
    }
}

void MaxHeapIncreaseKeyNoConstraint(int *number, int x, int key){
    if (key < number[x]) {
        number[x] = key;
        MaxHeapify(number, x);
    } else {
        number[x] = key;
        while ((x > 0) && (number[Parent(x)] < number[x])) {
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