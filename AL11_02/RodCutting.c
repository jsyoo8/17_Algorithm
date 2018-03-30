#include <stdio.h>
#include <stdlib.h>

#define INF 100000
#define MAX(A, B) ((A) > (B) ? (A) : (B))

int size; // rod 개수
int *s; // 최고 비용 인덱스 저장
int *r; // iterative에서 결과 r 배열

// 파일 읽기 (argc : 인자 개수, argv[1] 파일명)
int *readFile(int argc, char *argv[]) {
    int *p; // 각 cost 배열
    int i, dump; // i는 반복문 인덱스, dump는 각 행의 첫 문자 받는 용도
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
    p = calloc(size + 1, sizeof(int));
    s = calloc(size + 1, sizeof(int));
    // 1부터 size까지 파일의 문자 저장
    for (i = 1; i <= size; i++) {
        fscanf(file, "%d %d ", &dump, &(p[i]));
    }

    fclose(file);

    return p;
}

// 재귀적으로 실행하는 Rod Cutting
// cost 배열과 재귀적으로 돌아가는 n (초기값은 선택한 초기 막대)
int CutRod(int *p, int n) {
    int i, q, k;
    // n이 0이면 함수 종료
    if (n == 0) {
        return 0;
    }
    // q 초기값 -INF 설정
    q = -INF;
    // i는 1부터 n까지
    for (i = 1; i <= n; i++) {
        // k는 비교를 하기 위해서 따로 설정
        // 해당 i 막대와 재귀적으로 실행한 값의 합
        k = p[i] + CutRod(p, n - i);
        // q와 k 값 비교
        if (q < k) {
            // k가 더 크다면 q, s[n] 저장
            q = k;
            s[n] = i;
        }
        // q와 k값 중에서 큰 값이 q가 됨
        q = MAX(q, p[i] + CutRod(p, n - i));
    }
    return q;
}

// 반복적으로 실행하는 Rod Cutting
// cost 배열과 반복적으로 돌아가는 막대 n
void BottomUpCutRod(int *p, int n) {
    int i, j, q;
    // r 배열 n까지 할당
    r = calloc(n + 1, sizeof(int));
    // r[0]를 0으로 초기화
    r[0] = 0;
    // j=1부터 n까지 반복
    for (j = 1; j <= n; j++) {
        // q는 -INF 초기화
        q = -INF;
        // i=1부터 j까지 반복
        for (i = 1; i <= j; i++) {
            // q가 재귀와 같은 p[i] + r[j-1}보다 작다면
            if (q < p[i] + r[j - i]) {
                // q 값 새로 저장
                q = p[i] + r[j - i];
                // s 배열에 새로운 인덱스 값 저장
                s[j] = i;
            }
        }
        // r[j]는 q로 저장
        r[j] = q;
    }
}

// cut rod 출력 메소드
// cost 배열 p와 size n 입력
void PrintCutRodSolution(int *p, int n) {
    int i = n;
    printf("\nIndex : ");
    // index 출력 위한 첫 번째 반복문
    while (i > 0) {
        printf("%d ", s[i]);
        i = i - s[i];
    }
    printf("\nCost : ");
    i = n;
    // cost 출력 위한 두 번째 반복문
    while (i > 0) {
        printf("%d ", p[s[i]]);
        i = i - s[i];
    }
}

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./LCS.exe sample_lcs1.txt.txt
int main(int argc, char *argv[]) {
    int *p;
    int i, n;
    i = 4; // 재귀와 반복 두 번의 실행을 위한 n 지정 값
    size = 0; // 개수 초기화
    p = readFile(argc, argv); // 파일 읽어오기

    printf("시작");
    n = i;
    printf("\n재귀");
    CutRod(p, n); // 재귀 연산 실행
    PrintCutRodSolution(p, n); // 출력
    n = i;
    printf("\n반복");
    BottomUpCutRod(p, n); // 반복 연산 실행
    PrintCutRodSolution(p, n); // 출력
    printf("\n완료");

    free(p); // p 메모리 해제
    free(s); // s 메모리 해제
    free(r); // r 메모리 해제

    return 1;
}