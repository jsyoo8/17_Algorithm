#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 100000

int size; // 원소 개수
int **s; // 문제별
int **f; // 문제별 종료 배열
int **v; // 문제별 가치 배열
int **r; // 문제별 결과 r 배열
int **item; // sort 되더라도 원래 index를 찾게 하기 위한 문제별 배열

// 삽입 정렬
void selectionSort() {
    int i, j, q, *t;
    t = calloc(4, sizeof(int)); // 보기 편하게 구별함. 시작, 종료, 가치 배열을 교환해주는 매개 변수
    // 5번 문제는 정렬 되지 않은 배열을 필요
    for (q = 1; q < 5; q++) {
        if (q == 4) {
            // 종료 기준으로 정렬
            for (i = 1; i < size; i++) {
                for (j = 1; j < size - i; j++) {
                    if (s[q][j] > s[q][j + 1]) {
                        t[0] = s[q][j];
                        s[q][j] = s[q][j + 1];
                        s[q][j + 1] = t[0];
                        t[1] = f[q][j];
                        f[q][j] = f[q][j + 1];
                        f[q][j + 1] = t[1];
                        t[2] = v[q][j];
                        v[q][j] = v[q][j + 1];
                        v[q][j + 1] = t[2];
                        t[3] = item[q][j];
                        item[q][j] = item[q][j + 1];
                        item[q][j + 1] = t[3];
                    }
                }
            }
        } else {
            // 시작 기준으로 정렬
            for (i = 1; i < size; i++) {
                for (j = 1; j < size - i; j++) {
                    if (f[q][j] > f[q][j + 1]) {
                        t[0] = s[q][j];
                        s[q][j] = s[q][j + 1];
                        s[q][j + 1] = t[0];
                        t[1] = f[q][j];
                        f[q][j] = f[q][j + 1];
                        f[q][j + 1] = t[1];
                        t[2] = v[q][j];
                        v[q][j] = v[q][j + 1];
                        v[q][j + 1] = t[2];
                        t[3] = item[q][j];
                        item[q][j] = item[q][j + 1];
                        item[q][j + 1] = t[3];
                    }
                }
            }
        }
    }
}

// 배열 복사. from에서 to까지 n개만큼 복사하는 함수
// 복사된 함수를 반환
int *copyArray(int *to, int *from, int n) {
    int i = 0;
    for (i = 0; i < n; i++) {
        to[i] = from[i];
    };
    return to;
}

// 파일 읽기 (argc : 인자 개수, argv[1] 파일명)
void readFile(int argc, char *argv[]) {
    FILE *file; // 파일 포인터
    int i;
    size = 0;
    // 문제가 5개이고 1번부터 5번까지 할당. 0번은 기본 배열
    s = calloc(6, sizeof(int *));
    f = calloc(6, sizeof(int *));
    v = calloc(6, sizeof(int *));
    r = calloc(6, sizeof(int *));
    item = calloc(6, sizeof(int *));
    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }

    // 1부터 size까지 저장해야 하기에 size+1 할당
    s[0] = calloc(size + 1, sizeof(int));
    f[0] = calloc(size + 1, sizeof(int));
    v[0] = calloc(size + 1, sizeof(int));
    r[0] = calloc(size + 1, sizeof(int));
    item[0] = calloc(size + 1, sizeof(int));
    size++;
    // 1부터 size까지 파일의 숫자 저장
    while (!feof(file)) {
        s[0] = realloc(s[0], (size + 1) * sizeof(int));
        f[0] = realloc(f[0], (size + 1) * sizeof(int));
        v[0] = realloc(v[0], (size + 1) * sizeof(int));
        r[0] = realloc(r[0], (size + 1) * sizeof(int));
        item[0] = realloc(item[0], (size + 1) * sizeof(int));
        s[0][size] = 0;
        f[0][size] = 0;
        v[0][size] = 0;
        r[0][size] = 0;
        item[0][size] = size; // 정렬되어도 인덱스를 찾을 수 있도록 배정
        fscanf(file, "%d %d %d ", &(s[0][size]), &(f[0][size]), &(v[0][size]));
        size++;
    }
    s[0] = realloc(s[0], (size + 1) * sizeof(int));
    f[0] = realloc(f[0], (size + 1) * sizeof(int));
    v[0] = realloc(v[0], (size + 1) * sizeof(int));
    r[0] = realloc(r[0], (size + 1) * sizeof(int));
    item[0] = realloc(item[0], (size + 1) * sizeof(int));
    item[0][size] = size;
    s[0][size] = INF;

    for (i = 1; i < 6; i++) {
        s[i] = calloc(size + 1, sizeof(int));
        f[i] = calloc(size + 1, sizeof(int));
        v[i] = calloc(size + 1, sizeof(int));
        r[i] = calloc(size + 1, sizeof(int));
        item[i] = calloc(size + 1, sizeof(int));
        s[i] = copyArray(s[i], s[0], size + 1);
        f[i] = copyArray(f[i], f[0], size + 1);
        v[i] = copyArray(v[i], v[0], size + 1);
        item[i] = copyArray(item[i], item[0], size + 1);
    }
    // 정렬
    selectionSort();
    fclose(file);
}

// 반복적으로 실행하는 Activity Selection
// Greedy 기반 구현
// arr는 각 문제 번호
void ActivitySelectionGreedy(int arr) {
    int n, k, m, t;
    int *A = calloc(size + 1, sizeof(int));
    n = size;
    t = 1;
    A[t++] = item[arr][1];
    k = 1;
    for (m = 2; m < n; m++) {
        if (s[arr][m] >= f[arr][k]) {
            // 시작시간이 이전의 종료시간 이상인 경우
            A[t++] = item[arr][m];
            k = m;
        }
    }
    r[arr] = A; // 결과배열에 A 배정
}

// 메모리 해제
void freeMem(){
    int i;
    for (i = 0; i < 6; i++) {
        free(f[i]);
    }
    for (i = 0; i < 6; i++) {
        free(s[i]);
    }
    for (i = 0; i < 6; i++) {
        free(v[i]);
    }
    for (i = 0; i < 6; i++) {
        free(r[i]);
    }
    free(f); // 메모리 해제
    free(s); // 메모리 해제
    free(r); // 메모리 해제
    free(v); // 메모리 해제
}


//파일 실행 옵션으로 txt파일 이름 입력 ex) ./LCS.exe sample_lcs1.txt.txt
int main(int argc, char *argv[]) {
    int i, j;
    size = 0; // 개수 초기화
    readFile(argc, argv); // 파일 읽어오기

    printf("시작\n");
    for (i = 3; i <= 5; i++) {
        // 3번부터 5번문제까지 Greedy 기반으로 구현한 함수 실행
        ActivitySelectionGreedy(i);
        // 각 문제 실행결과 즉시 출력
        for (j = 1; j < size; j++) {
            if (r[i][j] == 0) {
                // 기본 값이 0이고 item은 1부터이기에 0인 기본값을 만나면 count를 출력하고 반복문을 넘김
                printf("\ncount : %d", (j - 1));
                break;
            }
            // 각 문제의 결과를 출력
            printf("%d\t", r[i][j]);
        }
        printf("\n");
    }
    printf("\n완료");
    freeMem();
    return 1;
}