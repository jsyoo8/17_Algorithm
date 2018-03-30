#include <stdio.h>
#include <stdlib.h>

void insert(int fScore, int* number, int size);
void insertSortScore(int* number, int size);
void printAll(int* number, int size);

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./testNumber.exe test_10.txt
int main(int argc, char* argv[]) {
    FILE* file;
    char* filename = argv[1];
    int fScore;
    int size = 0;
    int* number = calloc(20000, sizeof(int));

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if((file = fopen(filename, "r")) == NULL){
        printf( "파일이 열리지 않습니다.\n");
        exit(1);
    }

    while(feof(file)==0) {
        fscanf(file, "%d ", &fScore);
        insert(fScore, number, size);
        size++;
    }

    insertSortScore(number, size);

    printAll(number, size);
    fclose(file);
    printf("완료\n");
    free(number);
    return 0;
}

void insert(int fScore, int* number, int size){
    int score = fScore;
    number[size] = score;
}

void insertSortScore(int* number, int size){
    int i, j;
    int insertNode, curr, score;
    for (i = 1; i < size; i++) {
        insertNode = i;
        curr = 0;

        while (curr != insertNode) {
            if (number[curr] > number[insertNode]) {
                score = number[insertNode];
                for(j=0; j<(insertNode-curr); j++){
                    number[insertNode-j] = number[insertNode-j-1];
                }
                number[curr] = score;
                break;
            }
            curr++;
        }
    }
}

void printAll(int* number, int size){
    int i;
    for(i=0; i<size; i++){
        printf("%d \n", number[i]);
    }
}