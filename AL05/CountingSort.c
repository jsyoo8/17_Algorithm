#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void insert(int fScore, int *number);

void printAll(int *number, double timeComplexity);

void CountingSort(int *number, int *result);

int countingTimeComplexity;

int size;

int range;
//파일 실행 옵션으로 txt파일 이름 입력 ex) ./RandomQuick.exe test1.txt
int main(int argc, char *argv[]) {
    FILE *file;
    char *filename = argv[1];
    int fScore;
    double timeComplexity;
    time_t startTime = 0, endTime = 0;
    int *number = malloc(sizeof(int));
    int *result = malloc(sizeof(int));
    size = 0;
    range = 0;
    countingTimeComplexity = 0;

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

    result = realloc(result, (size)*sizeof(int));

    fclose(file);

    printf("시작\n");

    startTime = clock();
    CountingSort(number, result);
    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printAll(result, timeComplexity);

    printf("완료\n");
    free(number);
    free(result);
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
    printf("\n실행 시간 : %f(ms) \tcounting 횟수 : %d \n", timeComplexity, countingTimeComplexity);
}

void CountingSort(int *number, int *result){
    int i, num;
    int *histogram = calloc(1, sizeof(int));
    for(i=0;i<size;i++){
        num = number[i];
        countingTimeComplexity++;
        if(num > range){
            histogram = realloc(histogram, (num+1)*sizeof(int));
            memset(histogram + (range+1), 0, (num-range)*sizeof(int));
            range = num;
        }
        histogram[num]++;
    }

    for(i=1; i<=range; i++){
        countingTimeComplexity++;
        histogram[i] = histogram[i] + histogram[i-1];
    }
    for(i=size-1; i>=0; i--){
        countingTimeComplexity++;
        result[histogram[number[i]]-1] = number[i];
        histogram[number[i]]--;
    }

    free(histogram);
}