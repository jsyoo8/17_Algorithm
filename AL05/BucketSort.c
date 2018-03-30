#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct BucketList {
    float data;
    struct BucketList *next;
} bucketList;

typedef bucketList *pList;

void insert(float fScore, float *number);

void printAll(pList **pBox, double timeComplexity);

void printList(pList *list);

void BucketSort(float *number, pList **pBox);

void insertionSort(pList *ml);

pList *findIndex(pList *ml, int i);

int bucketTimeComplexity;

int size;

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./BucketSort.exe test1.txt
int main(int argc, char *argv[]) {
    FILE *file;
    char *filename = argv[1];
    float fScore;
    double timeComplexity;
    time_t startTime = 0, endTime = 0;
    float *number = malloc(sizeof(float));
    pList *pBox;
    size = 0;
    bucketTimeComplexity = 0;

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(filename, "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }

    while (feof(file) == 0) {
        fscanf(file, "%f\n", &fScore);
        number = realloc(number, (size + 1) * sizeof(float));
        insert(fScore, number);
        size++;
    }

    fclose(file);

    printf("시작\n");
    BucketSort(number, &pBox);
    startTime = clock();

    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printAll(&pBox, timeComplexity);

    printf("완료\n");
    free(number);
    free(pBox);
    return 1;
}

void insert(float fScore, float *number) {
    number[size] = fScore;
}

void printAll(pList **pBox, double timeComplexity) {
    int i;
    for (i = 0; i < size; i++) {
        printList(&((*pBox)[i]));
    }
    printf("\n실행 시간 : %f(ms) \tbucket 횟수 : %d \n", timeComplexity, bucketTimeComplexity);
}

void printList(pList *list) {
    pList *ml = list;
    if ((*ml) != NULL) {
        while ((*ml)->next != NULL) {
            printf("%.3f\t", (*ml)->data);
            ml = &((*ml)->next);
        }
        printf("%.3f\n", (*ml)->data);
    }
    printf("/\n");
}

void BucketSort(float *number, pList **pBox) {
    int i;
    pList *ml = NULL;
    (*pBox) = malloc(size * sizeof(pList));
    for (i = 0; i < size; i++) {
        (*pBox)[i] = NULL;
    }
    for (i = 0; i < size; i++) {
        ml = &(*pBox)[(int) floor(((float) size) * number[i])];
        if ((*ml) == NULL) {
            (*ml) = realloc((*ml), sizeof(bucketList));
            (*ml)->data = number[i];
            (*ml)->next = NULL;
        } else {
            while ((*ml)->next != NULL) {
                ml = &((*ml)->next);
            }
            (*ml)->next = realloc((*ml)->next, sizeof(bucketList));
            (*ml)->next->data = number[i];
            (*ml)->next->next = NULL;
        }
    }
    for (i = 0; i < size; i++) {
        insertionSort(&((*pBox)[i]));
    }
}

void insertionSort(pList *ml) {
    int i, j;
    pList *curr;
    pList *node;
    node = ml;
    int c;
    float num;

    if ((*ml) != NULL) {
        for (i = 1; (*node)->next != NULL; i++) {
            node = &((*node)->next);
            curr = ml;
            c = 0;

            while ((*curr) != (*node)) {
                if ((*curr)->data > (*node)->data) {
                    num = (*node)->data;
                    for (j = 0; j < (i - c); j++) {
                        (*findIndex(ml, i - j))->data = (*findIndex(ml, i - j - 1))->data;
                    }
                    (*curr)->data = num;
                    break;
                }
                curr = &((*curr)->next);
                c++;
            }
        }
    }
}

pList *findIndex(pList *ml, int i) {
    int index;
    pList *ptr = ml;
    for (index = 0; index < i; index++) {
        ptr = &((*ptr)->next);
    }
    return ptr;
}