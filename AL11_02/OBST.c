#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#define INF 100000

int size; // 행렬의 개수
float *p; // K 정보를 갖는 배열
float *q; // D 정보를 갖는 배열
float **e, **w; // OptimalBST에서 사용되는 배열
int **root; // root가 기록되는 배열
char **result; // 출력될 OBST가 기록되는 문자열 배열
int a; // D 배열의 인덱스

// 파일 읽기 (argc : 인자 개수, argv[1] 파일명)
void readFile(int argc, char *argv[]) {
    int i;
    FILE *file; // 파일 포인터

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }

    fscanf(file, "%d\n", &size); // 첫 번째 줄 데이터만 읽어오기
    // 1부터 size까지 저장해야 하기에 xSize+1 할당
    p = calloc(size + 1, sizeof(float));
    // 1부터 size까지 파일의 값 저장
    for (i = 1; i <= size; i++) {
        fscanf(file, "%f ", &(p[i]));
    }

    // 0부터 size까지 저장해야 하기에 xSize+1 할당
    q = calloc(size + 1, sizeof(float));
    // 0부터 size까지 파일의 값 저장
    for (i = 0; i <= size; i++) {
        fscanf(file, "%f ", &(q[i]));
    }

    fclose(file);
}

void OptimalBST(int n) {
    int i, l, j, r;
    float t;
    // n+1까지의 행이기에 n+2개 할당
    e = calloc(n + 2, sizeof(float *));
    // 1부터 n+1까지의 행에 n까지의 열을 할당
    for (i = 1; i <= n + 1; i++) {
        e[i] = calloc(n + 1, sizeof(float));
    }
    // n+1까지의 행이기에 n+2개 할당
    w = calloc(n + 2, sizeof(float *));
    // 1부터 n+1까지의 행에 n까지의 열을 할당
    for (i = 1; i <= n + 1; i++) {
        w[i] = calloc(n + 1, sizeof(float));
    }
    // n까지의 행이기에 n+1개 할당
    root = calloc(n + 1, sizeof(int *));
    // 1부터 n까지의 행에 n까지의 열을 할당
    for (i = 1; i <= n; i++) {
        root[i] = calloc(n + 1, sizeof(int));
    }
    // e와 w 배열 기초 값 배정
    for (i = 1; i <= n + 1; i++) {
        e[i][i - 1] = q[i - 1];
        w[i][i - 1] = q[i - 1];
    }
    // l이 1부터 n까지
    for (l = 1; l <= n; l++) {
        // i는 1부터 n-l+1까지
        for (i = 1; i <= n - l + 1; i++) {
            // 각 j의 인덱스 설정
            j = i + l - 1;
            // e의 초기값 INF 설정
            e[i][j] = INF;
            // w[i,j]의 연산 값 저장
            w[i][j] = w[i][j - 1] + p[j] + q[j];
            // r은 i부터 j까지 순회
            for (r = i; r <= j; r++) {
                // 순회하며 t값 비교
                t = e[i][r - 1] + e[r + 1][j] + w[i][j];
                if (t < e[i][j]) {
                    // t가 e[i,j]보다 작다면
                    e[i][j] = t; // e 배열에 t 배정
                    root[i][j] = r; // root 배열에 r 배정
                }
            }
        }
    }
}

// 왼쪽 자식을 찾는 함수
int left(int n) {
    return 2 * n;
}

// 오른쪽 자식을 찾는 함수
int right(int n) {
    return 2 * n + 1;
}

// OBST 출력을 위한 문자열 배열 생성
// 재귀적으로 작동, lc와 rc는 root의 왼쪽과 오른쪽 자식 값, i는 결과 result의 인덱스 값
void makeOBST(int lc, int rc, int i) {
    int r;
    // lc는 rc 이하, 더 크다면 해당 K재귀 정지
    if (lc <= rc) {
        // 안전장치, i의 범위는 1부터 2의 size승 까지
        if (i <= pow(2, size) && i > 0) {
            r = root[lc][rc]; // r은 root[rc][lc]
            sprintf(result[i], "k%d", r); // result[i]에 해당 k와 해당 r값 저장
            makeOBST(lc, r - 1, left(i)); // 왼쪽 자식 재귀
            makeOBST(r + 1, rc, right(i)); // 오른쪽 자식 재귀
        }
    } else {
        // 정지 후 D 정보 삽입
        sprintf(result[i], "d%d", a);
        // D의 인덱스 a
        a++;
    }
}


void printfOBST() {
    int i, j, k;
    a = 0; // a 초기화
    k = 1; // result의 초기 index
    // result 2의 size승까지 할당
    result = calloc(pow(2, size) + 1, sizeof(char *));
    // 각 문자열 크기 4 할당
    for (i = 0; i <= pow(2, size); i++) {
        result[i] = calloc(4, sizeof(char));
    }
    // OBST 출력 배열 제작
    makeOBST(1, size, 1);
    // OBST 출력
    // size level
    for (i = 0; i < size; i++) {
        printf("D%d : ", i);
        // 각 행별로 2의 j승 만큼 반복
        for (j = 0; j < pow(2, i); j++) {
            // result에 있는 문자열 존재여부 확인
            if (strlen(result[k]) > 0) {
                // 출력
                printf("%s\t", result[k++]);
            } else {
                // 없음
                printf("XX\t");
            }
        }
        printf("\n");
    }
}

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./LCS.exe sample_lcs1.txt.txt
int main(int argc, char *argv[]) {
    size = 0; // 개수 초기화
    readFile(argc, argv); // 파일 읽어오기

    printf("시작\n");
    OptimalBST(size); // OBST 연산
    printfOBST(); // OBST 출력
    printf("완료");

    free(p); // p 해제
    free(q); // q 해제
    free(e); // e 해제
    free(w); // w 해제
    free(root); // root 해제
    free(result); // result 해제

    return 1;
}