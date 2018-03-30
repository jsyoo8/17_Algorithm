#include <stdio.h>
#include <stdlib.h>

#define INF 99999
#define TRUE 1
#define FALSE 0

// 정점의 데이터들을 갖고 있는 구조체
typedef struct Vertex {
    int key;
    int vertex;
    int pi;
} vertex;

int input; // 시작 정점
int size; // 간선의 개수
int range; // 정점의 개수
vertex *vertexBox; // 정점 구조체 배열

// 파일 읽기 (argc : 인자 개수, argv[1] 파일명)
int **readFile(int argc, char *argv[]) {
    int **number; // cost를 담은 2차원 배열
    int vertex1, vertex2, cost;
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

// u 정점과 v정점과 cost가 담긴 2차원 배열을 입력받아 key 입력을 하는 함수
void Relax(vertex *u, vertex *v, int **number) {
    // v의 key가 u의 key와 (u,v)의 cost의 합보다 크다면 v의 key에 해당 값 삽입
    if (v->key > u->key + number[u->vertex][v->vertex]) {
        v->key = u->key + number[u->vertex][v->vertex];
        v->pi = u->vertex; // v의 부모 인덱스를 u로 설정
    }
}

// NegativeCycle이 존재하는지 탐색하는 함수. 정점 u, 정점 v, cost 2차원 배열 number를 입력받는다.
int isNegativeCycle(vertex *u, vertex *v, int **number) {
    // 현재위치와 최단거리 + 가중치를 비교
    if (v->key > u->key + number[u->vertex][v->vertex]) {
        // 바뀐다면 NegativeCycle 존재함으로 True 반환
        return TRUE;
    }
    // NegativeCycle가 아니기에 FALSE 반환
    return FALSE;
}

// Relax에서 배정한 pi를 이용하여 i 정점의 부모 정점을 재귀적으로 출력
void printShortestPath(vertex i) {
    // i의 부모 정점이 시작 정점인지 확인
    if (i.pi != input) {
        // 부모 정점이 시작 정점이 아닌 다른 정점이므로 재귀적 실행
        printShortestPath(vertexBox[i.pi]);
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
    printf("Vertex \t| Cost \t| Shortest Path\n");
    // 정점의 개수만큼 반복
    for (i = 0; i < range; i++) {
        // vertexBox에서 정점의 데이터들을 순서대로 출력
        printf("%d \t\t| %d \t\t| 0 ", i, vertexBox[i].key);
        // 최단경로를 출력하는 함수
        printShortestPath(vertexBox[i]);
        printf("\n");
    }
}

// BellmanFord함수 cost가 담긴 2차원 배열 number가 입력 됨
int BellmanFord(int **number) {
    int i, j, k;
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

    // n-1 만큼 반복
    for (i = 1; i < range; i++) {
        // 2차원 배열의 행
        for (j = 0; j < range; j++) {
            // 2차원 배열의 열
            for (k = 0; k < range; k++) {
                u = &(vertexBox[j]); // 행에 해당하는 정점
                v = &(vertexBox[k]); // 열에 해당하는 정점
                // 간선이 존재할 경우
                if (number[u->vertex][v->vertex] != INF) {
                    Relax(u, v, number); // Relax 함수 실행
                }
            }
        }
    }

    // Negative Cycle 탐색. 2차원 배열의 행과 열
    for (j = 0; j < range; j++) {
        for (k = 0; k < range; k++) {
            u = &(vertexBox[j]); // 행에 해당하는 정점
            v = &(vertexBox[k]); // 열에 해당하는 정점
            // 간선이 존재할 경우
            if (number[u->vertex][v->vertex] != INF) {
                // isNegativeCycle 함수 실행
                if (isNegativeCycle(u, v, number)) {
                    return FALSE; // Cycle을 형성한다면 FALSE 반환
                }
            }
        }
    }
    printAll(); // 출력

    return TRUE; // 성공 시 TRUE 반환
}


//파일 실행 옵션으로 txt파일 이름 입력 ex) ./BellmanFord.exe graph_sample_directed.txt
int main(int argc, char *argv[]) {
    int **number; // 파일에서 읽어 올 정수들
    input = 0; // 시작 정점 초기화
    size = 0; // 간선 개수 초기화
    number = readFile(argc, argv); // 파일 읽어오기
    vertexBox = calloc(range, sizeof(vertex)); // 정점 배열 초기화

    printf("시작\n");
    //BellmanFord 실행
    if (BellmanFord(number)) {
        // 성공
        printf("완료\n");
    } else {
        // 실패
        printf("실패");
    }
    free(number);
    free(vertexBox);
    return 1;
}
