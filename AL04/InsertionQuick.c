#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insert(int fScore, int *number);

void printAll(int *number, double timeComplexity);

void myQuicksort(int *number, int p, int r, int n);

int Partition(int *number, int p, int r);

void swap(int *number, int a, int b);

void insertSort(int *number, int left, int right);

int quickTimeComplexity;

int insertTimeComplexity;

int size;

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./InsertionQuick.exe test1.txt
int main(int argc, char *argv[]) {
    FILE *file;
    char *filename = argv[1];
    int fScore;
    double timeComplexity;
    time_t startTime = 0, endTime = 0;
    int *number = malloc(sizeof(int));
    int n = 100;
    size = 0;
    quickTimeComplexity = 0;
    insertTimeComplexity = 0;

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

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

    fclose(file);

    printf("시작\n");

    startTime = clock();
    myQuicksort(number, 0, size, n);
    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printAll(number, timeComplexity);

    printf("완료\n");
    free(number);
    return 1;
}

void insert(int fScore, int *number) {
    number[size] = fScore;
}

void printAll(int *number, double timeComplexity) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%d \t", number[i]);
    }
    printf("\n실행 시간 : %f(ms) \tquick 횟수 : %d \tinsert 횟수 : %d\n", timeComplexity, quickTimeComplexity,
           insertTimeComplexity);
}

void myQuicksort(int *number, int p, int r, int n) {
    int q;
    if (p < (r - 1)) {
        if ((r - p) > n) {
            q = Partition(number, p, r);
            myQuicksort(number, p, q, n);
            myQuicksort(number, q + 1, r, n);
        } else {
            insertSort(number, p, r);
        }
    }
}

int Partition(int *number, int p, int r) {
    int x = number[r - 1];
    int i = p - 1;
    int j;
    for (j = p; j < (r - 1); j++) {
        quickTimeComplexity++;
        if (number[j] <= x) {
            i++;
            swap(number, i, j);
        }
    }
    swap(number, i + 1, r - 1);
    return i + 1;
}

void insertSort(int *number, int left, int right) {
    int i, j;
    int insertNode, curr, score;

    for (i = left + 1; i < right; i++) {
        insertNode = i;
        curr = left;
        while (curr != insertNode) {
            insertTimeComplexity++;
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

void swap(int *number, int a, int b) {
    int i;
    if (number[a] > number[b]) {
        i = number[a];
        number[a] = number[b];
        number[b] = i;
    }
}