#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INF 99999

typedef struct Vertex {
    int parent;
    int key;
    int vertex;
} vertex;

typedef struct Edge {
    int vertex1;
    int vertex2;
    int cost;
} edge;

int size; // 간선의 개수
int range; // 정점의 개수
int heapSize; // heap의 개수
int *visited; // 방문한 점의 정보
int sum; // 사용한 간선 cost 합
edge *number; // 간선들의 힙
vertex *vertexBox; // 정점들의 데이터가 담긴 박스

// 파일 읽기 (argc : 인자 개수, argv[1] 파일명
edge *readFile(int argc, char *argv[]) {
    int vertex1, vertex2, cost; // 간선 구조체에 들어갈 데이터
    number = calloc(1, sizeof(edge)); // 간선 힙에 메모리 할당한다.
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


    // 두번 째 데이터부터 순서대로 모두 number에 삽입
    while (feof(file) == 0) {
        fscanf(file, "%d %d %d ", &vertex1, &vertex2, &cost);
        number = realloc(number, (size + 1) * sizeof(edge)); // 추가되는 간선만큼 메모리를 늘려서 할당한다.
        insert(vertex1, vertex2, cost, number);
        size++;
    }
    heapSize = size;
    fclose(file);

    BuildMinHeap();

    return number;
}

// fScore를 마지막 number에 삽입. 함수 종료시 size 하나 증가
void insert(int vertex1, int vertex2, int cost, edge *number) {
    number[size].cost = cost; // 간선 구조체에 cost를 저장한다.
    number[size].vertex1 = vertex1; // 간선 구조체에 vertex1 정보를 저장한다.
    number[size].vertex2 = vertex2; // 간선 구조체에 vertex2 정보를 저장한다.
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
            if (number[i].cost > number[lc].cost) {
                smallest = lc;
                swap(i, lc);
                MinHeapify(smallest);
            }
        } else {
            sc = (number[lc].cost < number[rc].cost) ? lc : rc;
            if (number[i].cost > number[sc].cost) {
                smallest = sc;
                swap(i, sc);
                MinHeapify(smallest);
            }
        }
    }
}

// 최소값을 힙에서 꺼낸다.
edge ExtractMin() {
    int i;
    edge min_value;
    BuildMinHeap();
    i = heapSize - 1;
    min_value = number[0];
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
    edge i;
    i = number[a];
    number[a] = number[b];
    number[b] = i;
}

// set을 초기화한다.
void MakeSet() {
    int i;
    for (i = 0; i < range; i++) {
        vertexBox[i].parent = -1;
        vertexBox[i].key = 1;
        vertexBox[i].vertex = i;
    }
}

// set의 최고 부모를 찾는 걸로 같은 집합을 확인한다. 최고 부모를 반환한다.
vertex *FindSet(vertex v) {
    int i, s;
    i = v.vertex;
    while (vertexBox[i].parent >= 0) {
        i = vertexBox[i].parent; // 최고 부모를 찾기 위해 i 배정을 계속 반복한다.
    }
    s = i; // 최고 부모를 s에 설정한다.
    i = v.vertex; // i에 다시 v를 배정한다.
    while (vertexBox[i].parent >= 0) {
        vertexBox[i].parent = s; // i의 부모를 s로 바꾼다.
        i = vertexBox[i].parent;
    }
    return &(vertexBox[s]); // s 정점의 포인터를 반환한다.
}

// u와 v가 들어있는 두 집합을 합친다.
void Union(vertex *u, vertex *v) {
    if (u->key < v->key) {
        u->parent = v->vertex; // u의 부모는 v로 설정한다.
        v->key += u->key; // v의 key에 u key값을 더한다.
    } else {
        v->parent = u->vertex; // v의 부모는 u로 설정한다.
        u->key += v->key; // u의 key에 v key값을 더한다.
    }
}

void kruskal() {
    int selected = 0; // 선택된 간선의 갯수
    vertex *u, *v; // vertex 포인터 u와 v
    edge e; // 간선 e

    MakeSet(); // union set을 초기화
    while (selected < range - 1) {
        e = ExtractMin(); // 간선 최소값을 뽑는다.
        u = FindSet(vertexBox[e.vertex1]); // vertex1의 set을 u에 저장
        v = FindSet(vertexBox[e.vertex2]); // vertex2의 set을 v에 저장
        if (u->vertex != v->vertex) { // u와 v가 다르다면
            printf("(%d,%d) %d \n", e.vertex1, e.vertex2, e.cost); // 간선의 정보를 출력한다.
            sum += e.cost; // sum에 cost를 더한다.
            selected++; // selected 증가
            Union(u, v); // u와 v를 합친다.
        }
    }
}


//파일 실행 옵션으로 txt파일 이름 입력 ex) ./kruskalMST.exe graph_sample.txt
int main(int argc, char *argv[]) {
    sum = 0;
    size = 0;
    number = readFile(argc, argv); // 파일 읽어오기
    visited = calloc(range, sizeof(int)); // visited를 0으로 초기화 시킨 값으로 메모리 할당한다.
    vertexBox = calloc(range, sizeof(vertex));// vertexBox에 메모리를 할당한다.

    printf("시작\n");
    kruskal(); // prim알고리즘을 실행한다.
    printf("Sum : %d\n완료\n", sum); // 총합을 출력한다.
    free(number);
    free(visited);
    free(vertexBox);
    return 1;
}