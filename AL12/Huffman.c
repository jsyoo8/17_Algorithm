#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 100000


typedef struct Node {
    struct Node *left;
    struct Node *right;
    int freq;
    char str;
    int code;
} node;

node *Q;
int **histo;
int size;
int count;
int qsize;


void sortHisto() {
    int i, j, t;
    for (i = 1; i < size + 1; i++) {
        for (j = 1; j < size + 1 - i; j++) {
            if (histo[1][j] > histo[1][j + 1]) {
                t = histo[0][j];
                histo[0][j] = histo[0][j + 1];
                histo[0][j + 1] = t;
                t = histo[1][j];
                histo[1][j] = histo[1][j + 1];
                histo[1][j + 1] = t;
            }
        }
    }
}


void sortQ() {
    int i, j;
    node t;
    for (i = 1; i <= qsize; i++) {
        for (j = 1; j <= qsize - i; j++) {
            if (Q[j].freq > Q[j + 1].freq) {
                t = Q[j];
                Q[j] = Q[j + 1];
                Q[j + 1] = t;
            }
        }
    }
}


void makeQ() {
    int i;
    sortHisto();
    Q = calloc(size + 1, sizeof(node));
    for (i = 1; i < size + 1; i++) {
        Q[i].str = histo[0][i];
        Q[i].freq = histo[1][i];
    }
}


void insertQ(node *p) {
    qsize++;
    copyNode(p, &(Q[qsize]));
    sortQ();
}


void copyNode(node *from, node *to) {
    (*to).freq = (*from).freq;
    (*to).left = (*from).left;
    (*to).code = (*from).code;
    (*to).right = (*from).right;
    (*to).str = (*from).str;
}


node *extractMin() {
    node *result = calloc(1, sizeof(node));
    copyNode(&(Q[1]), result);
    Q[1].freq = INF;
    Q[1].right = NULL;
    Q[1].left = NULL;
    Q[1].str = 0;
    Q[1].code = 0;
    sortQ();
    qsize--;
    return result;
}


int findIndex(char c) {
    int i;
    for (i = 1; i < size + 1; i++) {
        if (histo[0][i] == c) {
            return i;
        }
    }
    return -1;
}

void Histogram(char c) {
    int i;
    i = findIndex(c);
    if (i < 0) {
        size++;
        histo[0] = realloc(histo[0], (size + 1) * sizeof(int));
        histo[1] = realloc(histo[1], (size + 1) * sizeof(int));
        histo[0][size] = c;
        histo[1][size] = 1;
    } else {
        histo[1][i] += 1;
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
    char c;
    size = 0;
    count = 0;
    histo = calloc(2, sizeof(int *));
    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }

    // 1부터 size까지 저장해야 하기에 size+1 할당
    histo[0] = calloc(size + 1, sizeof(int));
    histo[1] = calloc(size + 1, sizeof(int));
    // 모든 문자를 읽고 Histogram화
    while (!feof(file)) {
        fscanf(file, "%c", &c);
        Histogram(c);
        count++;
    }
    fclose(file);
    // Q생성
    makeQ();
    qsize = size;
}

// 재귀적으로 노드들의 코드를 생성하는 함수
void encode(node *p, int code) {
    p->code = code;
    printf("%d\t", code);
    if (p->left != NULL) {
        encode(p->left, p->code * 10);
        encode(p->right, p->code * 10 + 1);
    }
}

// 허프만 코드의 중심 함수
// n은 p와 q를 합친 부모 노드
void Huffman() {
    node *n;
    node *p;
    node *q;
    while (qsize > 1) {
        p = extractMin(); // 제일 작은 node
        q = extractMin(); // 그다음 작은 node
        n = calloc(1, sizeof(node));
        n->freq = p->freq + q->freq;
        n->left = calloc(1, sizeof(node));
        copyNode(p, n->left);
        n->right = calloc(1, sizeof(node));
        copyNode(q, n->right);
        insertQ(n);
    }
    encode(n->left, 0); // 최종 결과에서 left 코드 부여
    encode(n->right, 1); // 최종 결과에서 right 코드 부여
}


void freeMem() {
    int i;
    for (i = 0; i < 2; i++) {
        free(histo[i]);
    }
    free(histo); // 메모리 해제
    free(Q); // 메모리 해제
}


//파일 실행 옵션으로 txt파일 이름 입력 ex) ./LCS.exe sample_lcs1.txt.txt
int main(int argc, char *argv[]) {
    int i, j;
    size = 0; // 개수 초기화
    readFile(argc, argv); // 파일 읽어오기

    printf("시작\n");
    Huffman();
    printf("\n완료");
    freeMem();
    return 1;
}