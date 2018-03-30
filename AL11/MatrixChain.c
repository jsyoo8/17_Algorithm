#include <stdio.h>
#include <stdlib.h>

#define INF 1000000

int size; // 행렬의 개수
int **m, **s; // m은 cost가 담기는 배열, s는 최적 수가 담기는 배열

// 파일 읽기 (argc : 인자 개수, argv[1] 파일명)
int *readFile(int argc, char *argv[]) {
    int *p; // cost를 담은 2차원 배열
    int i;
    FILE *file; // 파일 포인터
    i = 0;

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }
    fscanf(file, "%d\n", &size); // 첫 번째 줄 데이터만 읽어오기

    // 배열 p 할당 및 초기화
    // 0부터 n까지를 size로 받은 것이기에 할당 시 size+1만큼 할당
    p = calloc(size + 1, sizeof(int));

    // 순서대로 모두 p에 삽입
    while (feof(file) == 0) {
        fscanf(file, "%d ", &(p[i++]));
    }
    fclose(file);

    return p;
}

// 행렬의 최적 곱셈 cost 연산하는 함수
void MatrixChainOrder(int *p) {
    int n = size; // 0부터 n개 까지에서의 n
    int i, j, k, l, q;
    // m의 범위는 [1..n][1..n]이기에 n까지의 행을 위해서 n+1을 할당
    m = calloc(n + 1, sizeof(int *));
    // 1부터 n까지 행에서 n까지의 열을 할당
    for (i = 1; i < n + 1; i++) {
        m[i] = calloc(n + 1, sizeof(int));
    }
    // s의 범위는 [1..n-1][2..n]이기에 n-1까지의 행을 위해서 n을 할당
    s = calloc(n, sizeof(int *));
    // 1부터 n-1까지 행에서 n까지의 열을 할당
    for (i = 1; i < n; i++) {
        s[i] = calloc(n + 1, sizeof(int));
    }
    // calloc으로 할당 했기에 필요없지만 i,i를 0으로 초기화
    for (i = 1; i <= n; i++) {
        m[i][i] = 0;
    }
    // l은 체인 길이인데 2부터 n까지 증가
    for (l = 2; l <= n; l++) {
        // i는 1부터 n-l+1까지 증가
        // 해당 체인 길이에 대한 조합이 존재할 경우 반복
        for (i = 1; i <= n - l + 1; i++) {
            j = i + l - 1;
            // i,j를 비교를 위해 무한대로 설정
            m[i][j] = INF;
            // k를 기준으로 구간별로 나눔
            for (k = i; k <= j - 1; k++) {
                // 해당 행렬 곱 연산에서 cost를 계산하여 q에 저장
                q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                // q가 기존의 i,j 보다 작거나 기존의 i,j가 무한대라면 내부 실행
                if (q < m[i][j]) {
                    m[i][j] = q; // cost 저장
                    s[i][j] = k; // 더 적은 cost를 갖는 k 저장
                }
            }
        }
    }
}

// 재귀적으로 s 배열의 최적 cost 값을 갖는 기준 k값을 출력
void PrintOptimalParens(int i, int j) {
    // i와 j가 같아지는 경우 실행
    if (i == j) {
        printf("%d ", i);
    } else {
        // 같지 않은 경우
        // 최적 cost가 되는 위치를 이용하여 재귀적으로 실행
        printf("( ");
        PrintOptimalParens(i, s[i][j]);
        PrintOptimalParens(s[i][j] + 1, j);
        printf(") ");
    }
}

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./MatrixChain.exe sample_mat1.txt
int main(int argc, char *argv[]) {
    int *p; // 파일에서 읽어 올 정수들
    size = 0; // 행렬 개수 초기화
    p = readFile(argc, argv); // 파일 읽어오기

    printf("시작\n");
    MatrixChainOrder(p); // 계산 실행
    PrintOptimalParens(1, size); // 결과 출력
    printf("\ncost : %d", m[1][size]); // cost 출력
    printf("\n완료");

    free(p); // p 메모리 해제
    free(m); // m 메모리 해제
    free(s); // s 메모리 해제
    return 1;
}