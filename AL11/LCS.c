#include <stdio.h>
#include <stdlib.h>


int xSize, ySize; // 행렬의 개수
char *X, *Y; // 각 문자 배열
int **c; // 비교 후 저장 될 값이 들어갈 정수 배열
char **b; // 다음 포인트를 가리키는 문자 배열

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

    fscanf(file, "%d\n", &xSize); // 첫 번째 줄 데이터만 읽어오기
    // 1부터 xSize까지 저장해야 하기에 xSize+1 할당
    X = calloc(xSize + 1, sizeof(char));
    // 1부터 xSize까지 파일의 문자 저장
    for (i = 1; i <= xSize; i++) {
        fscanf(file, "%c", &(X[i]));
    }

    fscanf(file, "%d\n", &ySize); // 세 번째 줄 데이터만 읽어오기
    // 1부터 ySize까지 저장해야 하기에 xSize+1 할당
    Y = calloc(ySize + 1, sizeof(char));
    // 1부터 ySize까지 파일의 문자 저장
    for (i = 1; i <= ySize; i++) {
        fscanf(file, "%c", &(Y[i]));
    }

    fclose(file);
}

// LCS를 연산하는 함수
void LcsLength() {
    int m = xSize;
    int n = ySize;
    int i, j;
    // m까지의 행이기에 m+1개 할당
    b = calloc(m + 1, sizeof(char *));
    // 1부터 m까지의 행에 n까지의 열을 할당
    for (i = 1; i < m + 1; i++) {
        b[i] = calloc(n + 1, sizeof(char));
    }
    // m까지의 행이기에 m+1개 할당
    c = calloc(m + 1, sizeof(int *));
    // 1부터 m까지의 행에 n까지의 열을 할당
    for (i = 0; i < m + 1; i++) {
        c[i] = calloc(n + 1, sizeof(int));
    }
    // c배열에서 행이나 열이 0인 인덱스는 0값으로 초기화
    for (i = 1; i <= m; i++) {
        c[i][0] = 0;
    }
    for (j = 0; j <= n; j++) {
        c[0][j] = 0;
    }
    // 1부터 m까지 반복
    for (i = 1; i <= m; i++) {
        // 1부터 n까지 반복
        for (j = 1; j <= n; j++) {
            // X[i]와 Y[j] 값 비교
            if (X[i] == Y[j]) {
                // 같다면 c는 각 1씩 뺀 위치의 값에서 1 증가
                c[i][j] = c[i - 1][j - 1] + 1;
                // b배열에는 왼쪽 위 방향을 가리키는 ` 저장
                b[i][j] = '`';
            } else if (c[i - 1][j] >= c[i][j - 1]) {
                // i,j의 위가 왼쪽보다 크다면 위 값을 i,j에 저장
                c[i][j] = c[i - 1][j];
                // 위 방향을 가리키는 l 저장
                b[i][j] = 'l';
            } else {
                // 왼쪽이 더 크다면 왼쪽 값을 i,j에 저장
                c[i][j] = c[i][j - 1];
                // 왼쪽 방향을 가리키는 - 저장
                b[i][j] = '-';
            }
        }
    }
}

// 연산 완료된 LCS의 결과를 출력
void PrintLCS(int i, int j) {
    // i 혹은 j 가 0이 되면 종료
    if (i == 0 || j == 0) {
        return;
    }
    // b배열에서 i,j위치의 방향으로 비교
    if (b[i][j] == '`') {
        // 대각 방향일 경우 i-1,j-1로 재귀적 실행
        PrintLCS(i - 1, j - 1);
        // 해당 X[i] 출력
        printf("%c ", X[i]);
    } else if (b[i][j] == 'l') {
        // 위 방향일 경우 i-1,j로 재귀적 실행
        PrintLCS(i - 1, j);
    } else {
        // 왼쪽 방향일 경우 i,j-1로 재귀적 실행
        PrintLCS(i, j - 1);
    }
}

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./LCS.exe sample_lcs1.txt.txt
int main(int argc, char *argv[]) {
    xSize = 0; // 행렬 개수 초기화
    ySize = 0; // 행렬 개수 초기화
    readFile(argc, argv); // 파일 읽어오기

    printf("시작\n");
    LcsLength(); // 연산 시작
    PrintLCS(xSize, ySize); // 결과 출력
    printf("\n완료");

    free(X); // X 메모리 해제
    free(Y); // Y 메모리 해제
    free(b); // b 메모리 해제
    free(c); // c 메모리 해제

    return 1;
}