#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INF 99999

// 정점의 데이터들을 갖고 있는 구조체
typedef struct Vertex {
    int key;
    int vertex;
    int pi;
} vertex;

int input; // 시작 정점
int size; // 간선의 개수
int range; // 정점의 개수
int heapSize; // 현재 heap 개수
int *visited; // 방문한 정점 체크 배열
vertex *vertexBox; // 정점 구조체 배열

// 파일 읽기 (argc : 인자 개수, argv[1] 파일명)
int **readFile(int argc, char *argv[]) {
    int vertex1, vertex2, cost, i, j;
    int **number; // cost를 담은 2차원 배열
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
        number[i] = realloc(number[i], range * sizeof(int));
    }
    for (i = 0; i < range; i++) {
        for (j = 0; j < range; j++) {
            number[i][j] = INF; // 초기값 INF로 선언
        }
    }

    // 두번 째 데이터부터 순서대로 모두 number에 삽입
    while (feof(file) == 0) {
        fscanf(file, "%d %d %d ", &vertex1, &vertex2, &cost);
        insert(vertex1, vertex2, cost, number);
        size++;
    }
    fclose(file);

    return number;
}

// number에서 (vertex1, vertex2) 위치에 cost 삽입
void insert(int vertex1, int vertex2, int cost, int **number) {
    number[vertex1][vertex2] = cost; // 1방향이기 때문에 vertex1, vertex2 순서 중요
}

// 힙에서 i의 부모인덱스 찾는 함수
int Parent(int i) {
    return (int) floor((i - 1) / 2);
}

// 힙에서 i의 왼쪽자식 인덱스 찾는 함수
int Left(int i) {
    return (int) floor(2 * i + 1);
}

// 힙에서 i의 오른쪽자식 인덱스 찾는 함수
int Right(int i) {
    return (int) floor(2 * i + 2);
}

// 힙에서 a인덱스와 b인덱스 위치의 데이터를 교환하는 함수
void swap(int a, int b) {
    vertex i;
    i = vertexBox[a];
    vertexBox[a] = vertexBox[b];
    vertexBox[b] = i;
}

// i에서 최소 힙을 찾아 맞추는 함수
void MinHeapify(int i) {
    int lc;
    int rc;
    int smallest;
    int sc;
    lc = Left(i);
    rc = Right(i);
    if (lc < heapSize) {
        if (rc >= heapSize) {
            if (vertexBox[i].key > vertexBox[lc].key) {
                smallest = lc;
                swap(i, lc);
                MinHeapify(smallest);
            }
        } else {
            sc = (vertexBox[lc].key < vertexBox[rc].key) ? lc : rc;
            if (vertexBox[i].key > vertexBox[sc].key) {
                smallest = sc;
                swap(i, sc);
                MinHeapify(smallest);
            }
        }
    }
}

// 최소 힙으로 빌드하는 함수
void BuildMinHeap() {
    int i;
    for (i = Parent(heapSize - 1); i >= 0; i--) {
        MinHeapify(i);
    }
}

// 힙에서 최소값을 꺼내는 함수
vertex *ExtractMin() {
    int i;
    vertex *min_value;
    BuildMinHeap();
    i = heapSize - 1;
    swap(0, i);
    min_value = &(vertexBox[i]); // 교환을 미리 수행한 다음에 i 위치 저장
    heapSize--;
    MinHeapify(0);
    return min_value; // vertexBox[i]의 주소 반환
}

// u 정점과 v정점과 cost가 담긴 2차원 배열을 입력받아 key 입력을 하는 함수
void Relax(vertex *u, vertex *v, int **number) {
    // v의 key가 u의 key와 (u,v)의 cost의 합보다 크다면 v의 key에 해당 값 삽입
    if (v->key > u->key + number[u->vertex][v->vertex]) {
        v->key = u->key + number[u->vertex][v->vertex];
        v->pi = u->vertex; // v의 부모 인덱스를 u로 설정
    }
}

// heap으로 순서가 섞인 vertexBox에서 입력받은 num에 해당하는 배열에서의 정점 위치를 반환하는 함수
int findVertex(int num) {
    int i;
    // i가 0부터 정점의 개수만큼 반복
    for (i = 0; i < range; i++) {
        // vertexBox에서 i인덱스의 정점 값이 찾는 값과 같다면 i반환
        if (vertexBox[i].vertex == num) {
            return i;
        }
    }
    // 못 찾은 경우
    return i;
}

// Relax에서 배정한 pi를 이용하여 i 정점의 부모 정점을 재귀적으로 출력
void printShortestPath(vertex i) {
    // i의 부모 정점이 시작 정점인지 확인
    if (i.pi != input) {
        // 부모 정점이 시작 정점이 아닌 다른 정점이므로 재귀적 실행
        printShortestPath(vertexBox[findVertex(i.pi)]);
        // 재귀 수행 후 i 정점 출력
        printf("%d ", i.vertex);
    } else {
        // 부모 정점이 시작 정점인 경우 i 정점 출력
        printf("%d ", i.vertex);
    }
}

// vertex와 cost를 출력하는 함수
void printAll() {
    int i;
    printf("Vertex \t| Cost\n");
    // 정점의 개수만큼 반복
    for (i = 0; i < range; i++) {
        // 위에서 설명한 findVertex를 이용하여, 보기좋게 i와 i의 key부터 출력
        printf("%d \t\t| %d \t\t| 0 ", i, vertexBox[findVertex(i)].key);
        // 최단경로를 출력하는 함수
        printShortestPath(vertexBox[findVertex(i)]);
        printf("\n");

    }
}

// Dijkstra함수 cost가 담긴 2차원 배열 number가 입력 됨
void Dijkstra(int **number) {
    int i;
    vertex *u, *v;

    // 정점 개수만큼 반복
    for (i = 0; i < range; i++) {
        // vertexBox[i]에서 vertex 정보를 배정
        vertexBox[i].vertex = i;
        // vertexBox[i]에서 key의 초기값 INF 배정
        vertexBox[i].key = INF;
    }
    // 시작 정점의 key에 0 배정
    vertexBox[input].key = 0;

    // heap이 끝이 날 때까지 (모든 정점)
    while (heapSize > 0) {
        u = ExtractMin(); // key값이 제일 적은 정점 추출하여 u에 배정
        visited[u->vertex] = 1; // 정점 u를 방문한 정점으로 수정
        // 정점 개수만큼 반복
        for (i = 0; i < range; i++) {
            v = &(vertexBox[findVertex(i)]); // v에 index i에 해당하는 정점 배정
            // i인덱스에 방문한 적이 없으며,
            if (visited[i] == 0) {
                // 시작 정점에서 u에 갈 방안이 있으며,
                if (u->key != INF) {
                    // 간선이 존재할 경우
                    if (number[u->vertex][v->vertex] != INF) {
                        Relax(u, v, number); // Relax 함수 실행
                    }
                }
            }
        }
    }
    printAll(); // 출력
}


//파일 실행 옵션으로 txt파일 이름 입력 ex) ./Dijkstra.exe graph_sample_directed.txt
int main(int argc, char *argv[]) {
    int **number; // 파일에서 읽어 올 정수들
    input = 0; // 시작 정점
    size = 0; // 간선 개수 초기화
    number = readFile(argc, argv); // 파일 읽어오기
    vertexBox = calloc(range, sizeof(vertex)); // 정점 배열 초기화
    visited = calloc(range, sizeof(int)); // 방문 배열 초기화

    printf("시작\n");
    Dijkstra(number); // Dijkstra 실행
    printf("완료\n");
    free(number);
    free(vertexBox);
    free(visited);
    return 1;
}
