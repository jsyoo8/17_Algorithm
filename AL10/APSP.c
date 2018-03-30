#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INF 100000
#define MIN(A, B) ((A) < (B) ? (A) : (B))

int ***L1; // method slow
int ***L2; // method fast
int size; // 간선의 개수
int range; // 정점의 개수

// number에서 (vertex1, vertex2) 위치에 cost 삽입
void insert(int vertex1, int vertex2, int cost, int **number) {
    number[vertex1][vertex2] = cost; // 1방향이기 때문에 vertex1, vertex2 순서 중요
}

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

    // 2차원 배열 number 할당 및 초기화
    number = calloc(range, sizeof(int *));
    for (i = 0; i < range; i++) {
        number[i] = calloc(range, sizeof(int));
    }

    for (i = 0; i < range; i++) {
        for (j = 0; j < range; j++) {
            // 자기 자신이 아니면
            if (i != j) {
                number[i][j] = INF; // 초기값 INF로 선언
            }
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

// 3차원 배열을 동적으로 할당
int ***TriArr() {
    int i, j;
    int ***arr;

    // 3차원 배열 할당
    arr = (int ***) calloc(range, sizeof(int **));

    // 각각의 2차원 배열 할당
    for (i = 0; i < range; i++) {
        arr[i] = (int **) calloc(range, sizeof(int *));
    }

    // 각각의 1차원 배열 할당
    for (i = 0; i < range; i++) {
        for (j = 0; j < range; j++) {
            arr[i][j] = (int *) calloc(range, sizeof(int));
        }
    }
    return arr;
}

// 동적으로 할당 된 메모리 해제
void FreeArr() {
    int i, j;

    // 2차원 배열 개수만큼 반복
    for (i = 0; i < range; i++) {
        // 1차원 배열 개수만큼 반복
        for (j = 0; j < range; j++) {
            free(L1[i][j]);
            free(L2[i][j]);
        }
        free(L1[i]);
        free(L2[i]);
    }
    free(L1);
    free(L2);
}

// 2차원 배열 복사
// 복사되는 배열 arr, 복사하려는 배열 weight
void arrayCopy2(int **arr, int **weight) {
    int *p1, *p2;
    int i, j;

    for (i = 0; i < range; i++) {
        for (j = 0; j < range; j++) {
            p1 = &(arr[i][j]); // 복사되는 arr 위치의 포인터 p1
            p2 = &(weight[i][j]); // 복사하는 weight 위치의 포인터 p2
            *p1 = *p2;
        }
    }
}

// 최단 경로를 각 단계별로 실행 하는 함수
// 비교하려는 이전의 2차원 배열 Ln, cost들이 담겨있는 2차원 배열 W, 값 이용 및 출력값이 저장되는 2차원 배열 Lm
int **ExtendShortestPaths(int **Ln, int **W, int **Lm) {
    int i, j, k;

    for (i = 0; i < range; i++) {
        for (j = 0; j < range; j++) {
            Lm[i][j] = INF; // 출력할 배열 Lm의 기본값은 INF로 배정
            for (k = 0; k < range; k++) {
                // Lm[i][j] 보다 Ln[i][k] + W[k][j]가 작다면 Lm[i][j]에 삽입
                // 이 때 INF 보다 약간 작아진 값이 되어도 printAll에서 걸러지기 때문에 상관 없음
                Lm[i][j] = MIN(Lm[i][j], Ln[i][k] + W[k][j]);
            }
        }
    }
    return Lm;
}

// APSP의 method 중 bellman ford와 접근법이 동일한 알고리즘
// cost가 담겨있는 2차원 배열 입력
int ***SlowAllPairsShortestPaths(int **W) {
    int i;
    // L1의 시작 2차원 배열은 W를 복사
    arrayCopy2(L1[0], W);

    // from 2 to n-1인 반복문
    for (i = 1; i < range - 1; i++) {
        // 새로운 단계 L1[i]에 ExtendShortestPaths 실행
        L1[i] = ExtendShortestPaths(L1[i - 1], W, L1[i]);
    }
    return L1;
}

// APSP의 method 중 더 빠른 알고리즘
// cost가 담겨있는 2차원 배열 입력
int ***FasterAllPairsShortestPaths(int **W) {
    int i, max;
    // L2의 시작 2차원 배열은 W를 복사
    arrayCopy2(L2[0], W);
    // 반복문을 돌릴 횟수인 max lg(n-1)의 내림 값을 추출
    max = (int) (log(range - 1) / log(2));

    // lg(n-1) + 1 번 반복
    // 1, 2, 4, 8 대신 0부터 3까지를 사용
    for (i = 1; i <= max + 1; i++) {
        // 새로운 단계 L2[i]에 ExtendShortestPaths 실행
        L2[i] = ExtendShortestPaths(L2[i - 1], L2[i - 1], L2[i]);
    }
    return L2;
}


void printAll() {
    int i, j, k, max;
    // 반복문을 돌릴 횟수인 max lg(n-1)의 내림 값을 추출
    max = (int) (log(range - 1) / log(2));

    printf("\n");
    // 값이 저장되어 있는 모든 배열 출력
    // L1의 2차원 배열
    for (i = 0; i < range - 1; i++) {
        printf("L1[%d]\n", i);
        // 해당 2차원 배열의 1차원 배열
        for (j = 0; j < range; j++) {
            // 해당 1차원 배열의 원소
            for (k = 0; k < range; k++) {
                // INF/10 보다 작다면 숫자이고 크다면 INF로 출력
                if (L1[i][j][k] < (INF / 10)) {
                    printf("%d\t", L1[i][j][k]);
                } else {
                    printf("INF\t");
                }
            }
            printf("\n");
        }
    }

    // 위와 동일
    printf("\n");
    for (i = 0; i <= max + 1; i++) {
        printf("L2[%d]\n", i);
        for (j = 0; j < range; j++) {
            for (k = 0; k < range; k++) {
                if (L2[i][j][k] < (INF / 10)) {
                    printf("%d\t", L2[i][j][k]);
                } else {
                    printf("INF\t");
                }
            }
            printf("\n");
        }
    }
}


//파일 실행 옵션으로 txt파일 이름 입력 ex) ./BellmanFord.exe graph_sample_directed.txt
int main(int argc, char *argv[]) {
    int **number; // 파일에서 읽어 올 정수들
    int i;
    size = 0; // 간선 개수 초기화
    number = readFile(argc, argv); // 파일 읽어오기
    L1 = TriArr(); // 3차원 배열 할당
    L2 = TriArr(); // 3차원 배열 할당

    printf("시작\n");
    L1 = SlowAllPairsShortestPaths(number); // slow method 결과 저장
    L2 = FasterAllPairsShortestPaths(number); // fast method 결과 저장
    printAll(); // 출력
    printf("\n완료");

    // number의 메모리 해제
    for (i = 0; i < range; i++) {
        free(number[i]);
    }
    free(number);
    // L1,L2의 메모리 해제
    FreeArr();
    return 1;
}