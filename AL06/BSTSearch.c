#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct BSTs {
    int key;
    struct BSTs *p;
    struct BSTs *left;
    struct BSTs *right;
} bsts;

void insert(int fScore, int *number);

void printAll(bsts *root, double timeComplexity);

void TreeInsert(int *number, bsts **root);

bsts *SearchR(bsts *root, int k);

bsts *SearchI(bsts *root, int k);

int bstTimeComplexity;

void InorderTreeWalk(bsts *root);

int size;


//파일 실행 옵션으로 txt파일 이름 입력 ex) ./bstInsert.exe test1.txt
int main(int argc, char *argv[]) {
    FILE *file;
    char *filename = argv[1];
    int fScore;
    double timeComplexity;
    time_t startTime = 0, endTime = 0;
    int *number = malloc(sizeof(int));
    int findNumber = 5;
    bsts *root = NULL;
    size = 0;
    bstTimeComplexity = 0;

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(filename, "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }

    while (feof(file) == 0) {
        fscanf(file, "%d\n", &fScore);
        number = realloc(number, (size + 1) * sizeof(int));
        insert(fScore, number);
        size++;
    }

    fclose(file);

    TreeInsert(number, &root);

    printf("SearchR : %d\n", findNumber);

    startTime = clock();
    if (SearchR(root, findNumber) != NULL) {
        printf("있음\n");
    } else {
        printf("없음\n");
    }
    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printAll(root, timeComplexity);

    printf("완료\n");
    bstTimeComplexity = 0;
    printf("SearchI : %d\n", findNumber);

    startTime = clock();
    if (SearchI(root, findNumber) != NULL) {
        printf("있음\n");
    } else {
        printf("없음\n");
    }
    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printAll(root, timeComplexity);

    printf("완료\n");

    free(number);
    free(root);
    return 1;
}

void insert(int fScore, int *number) {
    number[size] = fScore;
}

void printAll(bsts *root, double timeComplexity) {
    int i;
    /*for (i = 0; i < size; i++) {
        printf("%d \t", number[i]);
    }*/
    InorderTreeWalk(root);
    printf("\n실행 시간 : %f(ms) \tbstTimeComplexity : %d \n", timeComplexity, bstTimeComplexity);
}

void TreeInsert(int *number, bsts **root) {
    int i, value;
    bsts **x, *y;

    for (i = 0; i < size; i++) {
        value = number[i];
        x = root;
        y = *root;
        while (*x != NULL) {
            y = *x;
            if (value < (*x)->key) {
                x = &((*x)->left);
            } else {
                x = &((*x)->right);
            }
        }
        *x = calloc(1, sizeof(bsts));
        (*x)->p = y;
        (*x)->key = value;

    }
}

void InorderTreeWalk(bsts *root) {
    if (root != NULL) {
        InorderTreeWalk(root->left);
        printf("%d\t", root->key);
        InorderTreeWalk(root->right);
    }
}

bsts *SearchR(bsts *root, int k) {
    bsts *x;
    x = root;
    if (x == NULL || k == x->key) {
        return x;
    }
    bstTimeComplexity++;
    if (k < x->key) {
        return SearchR(root->left, k);
    } else {
        return SearchR(root->right, k);
    }
}

bsts *SearchI(bsts *root, int k) {
    bsts **x;
    x = &root;
    while (*x != NULL && k != (*x)->key) {
        bstTimeComplexity++;
        if (k < (*x)->key) {
            x = &((*x)->left);
        } else {
            x = &((*x)->right);
        }
    }
    return *x;
}