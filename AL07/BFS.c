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
    struct UU *pi;
} U;

typedef struct Queue {
    int qSize;
    struct Item *front;
    struct Item *rear;
} queue;

int *readFile(int argc, char *argv[]);

void insert(int fScore, int *number);

void printAll(pItem **pBox, double timeComplexity);

void printList(pItem *list);

pItem *makeAdj(int *number, pItem **pBox);

void enqueue(int data);

int dequeue();

void BFS(pItem *pBox, int index);

int bstTimeComplexity;

int adjTimeComplexity;

int size;

int range;

queue *myQ;

U *visited;

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./DFS.exe test1.txt
int main(int argc, char *argv[]) {
    double timeComplexity; // 실행 시간
    int *number; // 파일에서 읽어 올 정수들
    pItem *pBox; // 인접행렬을 담을 포인터배열
    int i = 1;
    time_t startTime = 0, endTime = 0;
    size = 0;
    range = 0;
    bstTimeComplexity = 0; // bst 실행 횟수 체크
    adjTimeComplexity = 0; // 인접행렬 생성 횟수 체크
    myQ = calloc(1, sizeof(queue)); // 큐 생성
    number = readFile(argc, argv); // 파일 읽어오기
    pBox = makeAdj(number, &pBox); // 인접행렬 형성

    printf("시작\n");
    startTime = clock();
    BFS(pBox, i); // i에서 부터 BFS 실행
    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0; // 단위 상승 (*1000)

    printAll(&pBox, timeComplexity); // 저장된 인접행렬 체크 및 시간복잡도 체크

    printf("완료\n");
    free(number);
    free(pBox);
    free(myQ);
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
        number = realloc(number, (size + 1) * sizeof(float));
        insert(fScore, number);
        size++;
    }
    fclose(file);

    return number;
}

// fScore를 마지막 number에 삽입. 함수 종료시 size 하나 증가
void insert(int fScore, int *number) {
    number[size] = fScore;
}

// 실행 횟수, 실행 시간을 출력하고, 인접행렬을 printList를 활용하여 출력
void printAll(pItem **pBox, double timeComplexity) {
    int i;
    printf("\n실행 시간 : %f(ms) \tBFS : %d \tadj : %d \n", timeComplexity, bstTimeComplexity, adjTimeComplexity);
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

// 큐에 정수를 삽입하는 함수
void enqueue(int data) {
    item *temp = calloc(1, sizeof(item)); // 초기값을 갖는 item 포인터 temp 생성
    temp->data = data; // item의 값에 주어진 값 설정
    // 큐에 들어있는 값이 없을 때
    if (myQ->qSize == 0) {
        myQ->front = temp; // 제일 앞을 temp로 변경
        myQ->rear = temp; // 제일 뒤 또한 temp로 변경
    } else {
        // 큐에 값이 이미 있을 때
        myQ->rear->next = temp; // 제일 뒤의 다음을 temp로 연결
        myQ->rear = temp; // 제일 뒤를 temp로 설정
    }
    myQ->qSize = myQ->qSize + 1; // 큐의 사이즈 1증가
}

// 큐에서 가장 앞에 있던 정수 추출하는 함수
int dequeue() {
    item *temp = myQ->front; // item 포인터 temp를 큐의 제일 앞 노드로 설정
    int data; // 추출할 노드의 데이터 (반환 값)
    if (myQ->qSize != 0) {
        // 큐에 들어 있어야 추출 가능
        myQ->qSize = myQ->qSize - 1; // 사이즈 1감소
        data = temp->data; // 반환 할 data에 temp의 데이터 설정
        myQ->front = myQ->front->next; // 제일 앞 노드에서 그 다음 노드로 포인터 설정 변경
        if (myQ->front == NULL) {
            // 추출한 뒤에 큐에 남아있는 노드가 없다면 맨 뒤 포인터 또한 NULL로 초기화
            myQ->rear = NULL;
        }
        free(temp);
        return data;
    }
}

// BFS 함수 (pdf 수도코드 기반). 인접행렬 pBox와 처음 해당 index를 인자로 받음
void BFS(pItem *pBox, int index) {
    int i, u; // i는 반복문에 사용, u는 큐에서 추출한 정점 저장
    pItem *ml = NULL; // 인접행렬을 확인할 포인터
    for (i = 0; i < range; i++) {
        // 각 정점 구조체 배열을 초기화
        if (i != index) {
            visited[i].d = 10000;
        }
        bstTimeComplexity++;
    }
    visited[index].color = 1; // 회색 대신 숫자 사용. white : 0, black : 2
    visited[index].d = 0; // 확인된 정점의 초기설정
    enqueue(index); // 큐에 정점 삽입
    while (myQ->qSize != 0) {
        u = dequeue(); // 추출한 정수 u로 사용
        bstTimeComplexity++;
        printf("%d\t", u);
        ml = &(pBox[u]); // 추출한 정수 인접 행 포인터 설정
        while ((*ml) != NULL) {
            bstTimeComplexity++;
            if (visited[(*ml)->data].color == 0) {
                visited[(*ml)->data].color = 1; // 색 변경
                visited[(*ml)->data].d = visited[u].d + 1; // d 1증가
                visited[(*ml)->data].pi = &(visited[u]); // 연결된 부모 노드 설정
                enqueue((*ml)->data); // 해당 정점 큐에 삽입
            }
            ml = &((*ml)->next);
        }
        visited[u].color = 2; // u의 color black으로 변경
    }
}