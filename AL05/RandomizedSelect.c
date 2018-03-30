#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insert(int fScore, int *number);

void printAll(int *number, double timeComplexity);

int RandomizedSelect(int *number, int p, int r, int i);

int RandomizedPartition(int *number, int p, int r);

void swap(int *number, int a, int b);

int quickTimeComplexity;

int size;

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./RandomQuick.exe test1.txt
int main(int argc, char *argv[]) {
    FILE *file;
    char *filename = argv[1];
    int fScore;
    double timeComplexity;
    time_t startTime = 0, endTime = 0;
    int *number = malloc(sizeof(int));
    size = 0;
    quickTimeComplexity = 0;
    int i = 6; // i번째로 큰 값

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(filename, "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }

    while (feof(file) == 0) {
        fscanf(file, "%d\n", &fScore);
        number = realloc(number, (size + 1) * sizeof(int));
        insert(fScore, number);
        size++;
    }

    fclose(file);

    printf("시작\n");

    startTime = clock();
    printf("%d\n", RandomizedSelect(number, 0, size - 1, size - i + 1));
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
    printf("\n실행 시간 : %f(ms) \tquick 횟수 : %d \n", timeComplexity, quickTimeComplexity);
}

int RandomizedSelect(int *number, int p, int r, int i) {
    int q;
    int k;
    if (p < r) {
        q = RandomizedPartition(number, p, r);
        k = q - p + 1;
        if (i == k)
            return number[q];
        if (i < k) {
            RandomizedSelect(number, p, q - 1, i);
        } else {
            RandomizedSelect(number, q + 1, r, i - k);
        }
    } else {
        return number[p];
    }
}

int RandomizedPartition(int *number, int p, int r) {
    int x;
    int i = p - 1;
    int j;
    int randomNumber;
    srand((unsigned) time(NULL));
    randomNumber = rand() % (r - p) + p;
    x = number[randomNumber];
    number[randomNumber] = number[r];
    number[r] = x;

    for (j = p; j < r; j++) {
        quickTimeComplexity++;
        if (number[j] <= x) {
            i++;
            swap(number, i, j);
        }
    }
    swap(number, i + 1, r);
    return i + 1;
}


void swap(int *number, int a, int b) {
    int i;
    if (number[a] > number[b]) {
        i = number[a];
        number[a] = number[b];
        number[b] = i;
    }
}