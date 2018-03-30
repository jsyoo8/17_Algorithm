#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 100000
#define MAX(A, B) ((A) > (B) ? (A) : (B))

int Wi; // 정수 문제의 가방 최대값
float Wf; // 실수 문제의 가방 최대값
int size; // 가방에 들어가는 짐 개수
int *wi; // 정수 문제의 짐들 무게
float *wf; // 실수 문제의 짐들 무게
int *bi; // 정수 문제의 짐들 가치
int *bf; // 실수 문제의 짐들 가치
int **Bi; // 정수 문제의 결과 테이블 배열
int **Bf; // 실수 문제의 결과 테이블 배열

// 문제에서 요구한 배열과 값들을 미리 배정하는 함수 init
// 위의 전역변수들에서 설명한 것들을 문제의 요구사항에 맞게 배정한 것이기에 자세한 설명 생략
void init() {
    int i;
    Wi = 10;
    Wf = 1.0;
    size = 7;
    wi = calloc(size + 1, sizeof(int));
    wf = calloc(size + 1, sizeof(float));
    bi = calloc(size + 1, sizeof(int));
    bf = calloc(size + 1, sizeof(int));
    Bi = calloc(size + 1, sizeof(int *));
    for (i = 0; i <= size; i++) {
        Bi[i] = calloc(Wi, sizeof(int));
    }
    Bf = calloc(size + 1, sizeof(int *));
    for (i = 0; i <= size; i++) {
        Bf[i] = calloc(Wi, sizeof(int));
    }

    wi[1] = 1;
    wi[2] = 2;
    wi[3] = 3;
    wi[4] = 3;
    wi[5] = 4;
    wi[6] = 4;
    wi[7] = 5;
    bi[1] = 1;
    bi[2] = 3;
    bi[3] = 5;
    bi[4] = 6;
    bi[5] = 8;
    bi[6] = 9;
    bi[7] = 11;
    wf[1] = 0.20;
    wf[2] = 0.15;
    wf[3] = 0.25;
    wf[4] = 0.13;
    wf[5] = 0.22;
    wf[6] = 0.27;
    wf[7] = 0.30;
    bf[1] = 3;
    bf[2] = 2;
    bf[3] = 4;
    bf[4] = 2;
    bf[5] = 3;
    bf[6] = 4;
    bf[7] = 5;
}

// 정수 문제의 0-1 배낭 문제
// 테이블인 B를 Bi에 저장
void KnapsackI() {
    int w, i;
    int **B;
    B = calloc(size + 1, sizeof(int *));
    for (i = 0; i <= size; i++) {
        B[i] = calloc(Wi + 1, sizeof(int));
    }
    // i는 1부터 size까지
    for (i = 1; i <= size; i++) {
        // w는 1부터 가방의 최대값까지
        for (w = 1; w <= Wi; w++) {
            if (wi[i] <= w) {
                // 무게가 w보다 작거나 같다면
                if (bi[i] + B[i - 1][w - wi[i]] > B[i - 1][w]) {
                    // 가치와 B[i - 1][w - wi[i]]의 합이 B[i - 1][w]보다 크다면
                    B[i][w] = bi[i] + B[i - 1][w - wi[i]];
                } else {
                    // 작다면
                    B[i][w] = B[i - 1][w];
                }
            } else {
                // 무게가 w보다 크다면
                B[i][w] = B[i - 1][w];
            }
        }
    }
    Bi = B;
}

// 테이블의 해당되는 값을 출력하는 함수
void printI() {
    int i, j;
    /*
     * Dynamic Programming의 결과로 나온 테이블을 확인하고 싶을 경우 사용
    for (j = 0; j <= Wi; j++) {
        for (i = 0; i <= size; i++) {
            printf("%d\t", Bi[i][j]);
        }
        printf("\n");
    }
     */
    printf("%d\n", Bi[size][Wi]);
}

// 실수 문제의 0-1 배낭 문제
// 테이블인 B를 Bf에 저장
// 100은 소수점 둘째 자리까지인 실수이기에 정수로 변환하기 위해서 곱함
void KnapsackF() {
    int w, i;
    int **B;
    B = calloc(size + 1, sizeof(int *));
    for (i = 0; i <= size; i++) {
        B[i] = calloc(Wf * 100 + 1, sizeof(int));
    }
    // i는 1부터 size까지
    for (i = 1; i <= size; i++) {
        // w는 1부터 가방의 최대값까지
        for (w = 1; w <= Wf * 100; w++) {
            if (wf[i] * 100 <= w) {
                // 무게가 w보다 작거나 같다면
                if (bf[i] + B[i - 1][w - (int) (wf[i] * 100)] > B[i - 1][w]) {
                    // 가치와 B[i - 1][w - wf[i]*100]의 합이 B[i - 1][w]보다 크다면
                    B[i][w] = bf[i] + B[i - 1][w - (int) (wf[i] * 100)];
                } else {
                    // 작다면
                    B[i][w] = B[i - 1][w];
                }
            } else {
                // 무게가 w보다 크다면
                B[i][w] = B[i - 1][w];
            }
        }
    }
    Bf = B;
}

// 테이블의 해당되는 값을 출력하는 함수
void printF() {
    int i, j;
    /*
     * Dynamic Programming의 결과로 나온 테이블을 확인하고 싶을 경우 사용
    for (j = 0; j <= Wf*100; j++) {
        for (i = 0; i <= size; i++) {
            printf("%.2f\t", (float)Bf[i][j]);
        }
        printf("\n");
    }*/
    printf("%.2f\t", (float) Bf[size][(int) Wf * 100]);
}

void freeMem() {
    free(wi); // 메모리 해제
    free(bi); // 메모리 해제
    free(wf); // 메모리 해제
    free(bf); // 메모리 해제
    free(Bi); // 메모리 해제
    free(Bf); // 메모리 해제
}


int main() {
    init();
    printf("시작\n");
    KnapsackI();
    printI();
    KnapsackF();
    printF();
    printf("\n완료");
    freeMem();
    return 1;
}