#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INF 99999

// 정점의 데이터를 담은 구조체
typedef struct Vertex {
    int key;
    int vertex;
    int pi;
} vertex;

int size; // 간선의 개수
int range; // 정점의 개수
int heapSize; // heap의 개수
int *visited; // 방문한 점의 정보
int sum; // 사용한 간선 cost 합
vertex *heap; // 정점들의 힙
vertex *vertexBox; // 정점들의 데이터가 담긴 박스 heap에서 섞이는 정점의 인덱스 때문에 추가하였다.

// 파일 읽기 (argc : 인자 개수, argv[1] 파일명
int **readFile(int argc, char *argv[]) {
    int **number; // 2중 배열로 cost 정보를 받음
    int vertex1, vertex2, cost; // 간선 정보
    int i, j;
    FILE *file; // 파일 포인터

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }
    fscanf(file, "%d\n", &cost); // 첫 번째 줄 데이터만 읽어오기
    range = cost; // 정점 개수
    heapSize = range;

    number = calloc(range, range * sizeof(int));
    for (i = 0; i < range; i++) {
        number[i] = realloc(number[i], range * sizeof(int)); // number 는 각 정점들의 cost 정보를 갖고있는 2차원 배열이기 때문에 이렇게 선언한다.
    }
    for (i = 0; i < range; i++) {
        for (j = 0; j < range; j++) {
            if (i != j) {
                number[i][j] = INF; // cost가 없는 부분을 INF로 선언한다.
            }
        }
    }

    // 두번 째 데이터부터 순서대로 모두 number에 삽입
    while (feof(file) == 0) {
        fscanf(file, "%d %d %d ", &vertex1, &vertex2, &cost);
        insert(vertex1, vertex2, cost, number); // insert 함수를 통해 삽입한다.
        size++;
    }
    fclose(file);

    return number;
}

// fScore를 마지막 number에 삽입. 함수 종료시 size 하나 증가
void insert(int vertex1, int vertex2, int cost, int **number) {
    number[vertex1][vertex2] = cost;
    number[vertex2][vertex1] = cost; // 양방향이기 때문에 두번한다.
}

// Min Heap으로 빌드하는 함수 이전 과제를 응용한다.
void BuildMinHeap() {
    int i;
    for (i = Parent(heapSize - 1); i >= 0; i--) {
        MinHeapify(i);
    }
}

// Min Heapify 로 해당되는 값으로 값의 위치를 바꿔준다.
void MinHeapify(int i) {
    int lc;
    int rc;
    int smallest;
    int sc;
    lc = Left(i);
    rc = Right(i);
    if (lc < heapSize) {
        if (rc >= heapSize) {
            if (heap[i].key > heap[lc].key) {
                smallest = lc;
                swap(i, lc);
                MinHeapify(smallest);
            }
        } else {
            sc = (heap[lc].key < heap[rc].key) ? lc : rc;
            if (heap[i].key > heap[sc].key) {
                smallest = sc;
                swap(i, sc);
                MinHeapify(smallest);
            }
        }
    }
}

// 최소값을 힙에서 꺼낸다.
vertex ExtractMin() {
    int i;
    vertex min_value;
    BuildMinHeap();
    i = heapSize - 1;
    min_value = heap[0];
    swap(0, i);
    heapSize--;
    MinHeapify(0);
    return min_value;
}

// 부모 인덱스를 반환하는 함수
int Parent(int i) {
    return floor((i - 1) / 2);
}

// 왼쪽 자식의 인덱스를 반환하는 함수
int Left(int i) {
    return floor(2 * i + 1);
}

// 오른쪽 자식의 인덱스를 반환하는 함수
int Right(int i) {
    return floor(2 * i + 2);
}

// 힙에서 a 인덱스의 위치의 vertex와 b 인덱스의 위치의 vertex를 바꾸는 함수이다.
void swap(int a, int b) {
    vertex i;
    i = heap[a];
    heap[a] = heap[b];
    heap[b] = i;
}

// prim 알고리즘 cost 배열과 시작 정점인 input 값을 받고 프림알고리즘을 실행한다.
void MSTPrim(int **number, int input) {
    int i, j, u, v;

    // 정점배열과 heap을 초기화하면서 각 정점 구조체에 해당 정점 데이터를 입력한다.
    for (i = 0; i < range; i++) {
        heap[i].key = INF;
        heap[i].vertex = i;
        vertexBox[i].key = INF;
        vertexBox[i].vertex = i;
    }


    heap[input].key = 0; // 처음 시작하는 heap 정점의 key의 값을 0으로 설정한다.
    vertexBox[input].key = 0; // vertexBox 또한 0으로 설정한다.
    BuildMinHeap(); // heap을 빌드한다.
    for (i = 0; i < range; i++) {
        u = ExtractMin().vertex; // 힙에서 가장 작은 값의 key를 가진 정점을 u로 배정한다.
        visited[u] = 1; // u 정점은 방문했었다고 체크한다.
        if (vertexBox[u].key == INF) {
            return;
        }
        if (i == 0) {
            printf("PATH\t|\tCOST\n%d\t\t|\n", u); // 처음 실행하는 반복문에서 출력 틀을 설정한다.
        } else {
            printf("%d\t", u); // u를 출력한다.
            printf("\t|\t%d\n", vertexBox[u].key); // u의 key를 출력한다.
            sum += vertexBox[u].key; // key를 총합에 추가한다.
        }
        for (v = 0; v < range; v++) {
            if (number[u][v] != INF) { // 해당 코스트가 기본값인 INF가 아니라면
                if (!visited[v] && number[u][v] < vertexBox[v].key) { // v를 방문한 적이 없을 때 v의 key가 u,v의 cost보다 크다면
                    vertexBox[v].key = number[u][v]; // v의 key에 해당 cost를 저장한다.
                    vertexBox[v].pi = u; // v의 pi에 u를 설정한다.
                    for (j = 0; j < range; j++) {
                        if (heap[j].vertex == v) {
                            heap[j].key = number[u][v]; // heap의 key에도 cost로 수정한다.
                        }
                    }
                }
            }
        }
    }
}


//파일 실행 옵션으로 txt파일 이름 입력 ex) ./primMST.exe graph_sample.txt
int main(int argc, char *argv[]) {
    int **number; // 파일에서 읽어 올 정수들
    int input = 0;
    sum = 0;
    size = 0;
    number = readFile(argc, argv); // 파일 읽어오기
    visited = calloc(range, sizeof(int)); // visited를 0으로 초기화 시킨 값으로 메모리 할당한다.
    heap = calloc(range, sizeof(vertex)); // heap에 메모리를 할당한다.
    vertexBox = calloc(range, sizeof(vertex)); // vertexBox에 메모리를 할당한다.

    printf("시작\n");
    MSTPrim(number, input); // prim알고리즘을 실행한다.
    printf("Sum : %d\n완료\n", sum); // 총합을 출력한다.
    free(number);
    free(visited);
    free(heap);
    return 1;
}
