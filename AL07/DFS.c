#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Item {
    int data;
    struct Item *next;
} item;

typedef item *pItem;

typedef struct UU {
    int d;
    int color;
    int f;
    struct UU *pi;
} U;

int *readFile(int argc, char *argv[]);

void insert(int fScore, int *number);

void printAll(pItem **pBox, double timeComplexity);

void printList(pItem *list);

pItem *makeAdj(int *number, pItem **pBox);

void DFS(pItem *pBox);

void DFSVisit(pItem *pBox, int uIndex);

int dfsTimeComplexity;

int adjTimeComplexity;

int mySize;
int range;
int count;
U *visited;

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./DFS.exe test1.txt
int main(int argc, char *argv[]) {
    double timeComplexity; // 실행 시간
    int *number; // 파일에서 읽어 올 정수들
    pItem *pBox; // 인접행렬을 담을 포인터배열
    time_t startTime = 0, endTime = 0;
    mySize = 0;
    range = 0;
    count = 0;
    dfsTimeComplexity = 0; // bst 실행 횟수 체크
    adjTimeComplexity = 0; // 인접행렬 생성 횟수 체크
    number = readFile(argc, argv); // 파일 읽어오기
    pBox = makeAdj(number, &pBox); // 인접행렬 형성

    printf("시작\n");
    startTime = clock();
    DFS(pBox);
    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0; // 단위 상승 (*1000)

    printAll(&pBox, timeComplexity); // 저장된 인접행렬 체크 및 시간복잡도 체크

    printf("완료\n");
    free(number);
    free(pBox);
    return 1;
}

// 파일 읽기 (argc : 인자 개수, argv[1] 파일명
int *readFile(int argc, char *argv[]) {
    int *number = malloc(sizeof(float));
    int fScore;
    FILE *file; // 파일 포인터

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }
    fscanf(file, "%d\n", &fScore); // 첫 번째 줄 데이터만 읽어오기
    range = fScore; // 정점 개수

    visited = calloc(range, sizeof(U)); // 각 정점의 데이터가 담긴 구조체 생성 및 초기화

    // 두번 째 데이터부터 순서대로 모두 number에 삽입
    while (feof(file) == 0) {
        fscanf(file, "%d ", &fScore);
        number = realloc(number, (mySize + 1) * sizeof(float));
        insert(fScore, number);
        mySize++;
    }
    fclose(file);

    return number;
}

// fScore를 마지막 number에 삽입. 함수 종료시 size 하나 증가
void insert(int fScore, int *number) {
    number[mySize] = fScore;
}

// 실행 횟수, 실행 시간을 출력하고, 인접행렬을 printList를 활용하여 출력
void printAll(pItem **pBox, double timeComplexity) {
    int i;
    printf("\n실행 시간 : %f(ms) \tDFS : %d \tadj : %d \n", timeComplexity, dfsTimeComplexity, adjTimeComplexity);
    for (i = 0; i < range; i++) {
        // 각 인접 행렬의 행들을 배열의 인덱스로 하여 printList 실행
        printList(&((*pBox)[i]));
    }
}

// 인접 행렬의 각 행을 출력하는 함수
// ml은 현재 포인터가 가리키는 노드
void printList(pItem *list) {
    pItem *ml = list;
    if ((*ml) != NULL) {
        while ((*ml)->next != NULL) {
            printf("%d\t", (*ml)->data);
            ml = &((*ml)->next);
        }
        printf("%d\t", (*ml)->data);
    }
    printf("/\n");
}

// 인접행렬을 포인터 배열형태로 생성하는 함수
pItem *makeAdj(int *number, pItem **pBox) {
    int i, j;
    pItem *ml = NULL;
    (*pBox) = malloc(range * sizeof(pItem)); // 각 행들에 메모리 할당
    for (i = 0; i < range; i++) {
        (*pBox)[i] = NULL; // 각 행들의 초기화
    }
    for (j = 0; j < range; j++) {
        for (i = 0; i < range; i++) {
            adjTimeComplexity++;
            ml = &(*pBox)[j]; // 각 행들별로 노드를 가리키는 포인터 설정
            if (number[j * range + i] == 1) { // 각 행에서 1인 값만 링크드리스트로 추가
                if ((*ml) == NULL) {
                    // 현재 행이 NULL로 비어있을 때
                    (*ml) = realloc((*ml), sizeof(item));
                    (*ml)->data = i; // 이 때 노드에 해당 정점을 입력하기 위해 그냥 i를 사용
                    (*ml)->next = NULL;
                } else {
                    // 현재 행에 값이 있을때 마지막 위치로 포인터 설정
                    while ((*ml)->next != NULL) {
                        ml = &((*ml)->next);
                    }
                    (*ml)->next = realloc((*ml)->next, sizeof(item));
                    (*ml)->next->data = i;
                    (*ml)->next->next = NULL;
                }
            }
        }
    }
    return *pBox;
}

// DFS 함수 (pdf 수도코드 기반). 인접행렬 pBox를 인자로 받음
void DFS(pItem *pBox) {
    int i;
    for (i = 0; i < range; i++) {
        dfsTimeComplexity++;
        if (visited[i].color == 0) {
            // color가 white인 지점만 DFS Visit 실행
            DFSVisit(pBox, i);
        }
    }
}

//DFS Visit은 재귀적으로 실행되는 함수로 인접행렬 pBox, DFS 함수의 uIndex를 입력 인자로 받는다.
void DFSVisit(pItem *pBox, int uIndex) {
    pItem *ml = NULL; // 인접 행의 노드를 가리키는 포인터
    ml = &(pBox[uIndex]); // uIndex 행을 가리키게 설정
    count = count + 1; // count 1증가
    visited[uIndex].d = count; // d를 count로 설정
    visited[uIndex].color = 1; // color를 회색으로 설정
    printf("(%d\t", uIndex); // pdf 결과와 같이 출력하도록 맞춘 양식
    while ((*ml) != NULL) {
        dfsTimeComplexity++;
        if (visited[(*ml)->data].color == 0) { // color가 white인 정점에서 수행
            visited[(*ml)->data].pi = &(visited[uIndex]); // 연결된 부모 노드 설정
            DFSVisit(pBox, (*ml)->data); // 해당 정점에 대하여 DFSVisit 실행
        }
        ml = &((*ml)->next);
    }
    visited[uIndex].color = 2; // u의 color를 black으로 변경
    count = count + 1; // count 증가
    visited[uIndex].f = count; // f를 증가된 count로 설정
    printf("%d)\t", uIndex); // pdf 결과와 같이 출력하도록 맞춘 양식
}