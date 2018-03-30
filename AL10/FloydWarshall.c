#include <stdio.h>
#include <stdlib.h>

#define INF 100000
#define MIN(A, B) ((A) < (B) ? (A) : (B))

int ***D; // 각 단계를 담을 3차원 배열
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
    arr = (int ***) calloc(range + 1, sizeof(int **));

    // 각각의 2차원 배열 할당
    for (i = 0; i <= range; i++) {
        arr[i] = (int **) calloc(range, sizeof(int *));
    }

    // 각각의 1차원 배열 할당
    for (i = 0; i <= range; i++) {
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
    for (i = 0; i <= range; i++) {
        // 1차원 배열 개수만큼 반복
        for (j = 0; j < range; j++) {
            free(D[i][j]);
        }
        free(D[i]);
    }
    free(D);
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

// cost가 담겨있는 2차원 배열 입력
int ***FloydWarshall(int **W) {
    int i, j, k, d;
    // L1의 시작 2차원 배열은 W를 복사
    arrayCopy2(D[0], W);

    printf("\n");
    d = 1; // 알고리즘의 실행 단계. max : range
    for (k = 0; k < range; k++) {
        for (i = 0; i < range; i++) {
            for (j = 0; j < range; j++) {
                // D[d - 1][i][j] 보다 D[d - 1][i][k] + D[d - 1][k][j]가 작다면 Lm[i][j]에 삽입
                // 이 때 INF 보다 약간 작아진 값이 되어도 printAll에서 걸러지기 때문에 상관 없음
                D[d][i][j] = MIN(D[d - 1][i][j], D[d - 1][i][k] + D[d - 1][k][j]);
            }
        }
        d++;
    }
    return D;
}


void printAll() {
    int i, j, k;
    // 값이 저장되어 있는 모든 배열 출력
    // D의 2차원 배열
    for (i = 0; i <= range; i++) {
        printf("D[%d]\n", i);
        // 해당 2차원 배열의 1차원 배열
        for (j = 0; j < range; j++) {
            // 해당 1차원 배열의 원소
            for (k = 0; k < range; k++) {
                // INF/10 보다 작다면 숫자이고 크다면 INF로 출력
                if (D[i][j][k] < (INF / 10)) {
                    printf("%d\t", D[i][j][k]);
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
    D = TriArr(); // 3차원 배열 할당

    printf("시작\n");
    D = FloydWarshall(number);
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