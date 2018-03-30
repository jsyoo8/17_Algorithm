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

void insertionSort(int *number);

void TreeInsert(int *number, bsts **root);

void SortedArrayTreeInsert(int *number, bsts **root2);

bsts *sortedArrayToBST(int *number, int p, int r);

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
    bsts *root = NULL;
    bsts *root2 = NULL;
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

    printf("insert\n");

    startTime = clock();
    TreeInsert(number, &root);
    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printAll(root, timeComplexity);

    printf("완료\n");
    bstTimeComplexity = 0;
    printf("insert insertion sorted array\n");

    startTime = clock();
    insertionSort(number);
    SortedArrayTreeInsert(number, &root2);
    endTime = clock();

    timeComplexity = (double) (endTime - startTime) / (CLOCKS_PER_SEC);

    timeComplexity *= 1000.0;

    printAll(root2, timeComplexity);

    printf("완료\n");


    free(number);
    free(root);
    free(root2);
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
    printf("\n실행 시간 : %f(ms) \tinsert : %d \n", timeComplexity, bstTimeComplexity);
}

void TreeInsert(int *number, bsts **root) {
    int i, value;
    bsts **x, *y;

    for (i = 0; i < size; i++) {
        value = number[i];
        x = root;
        y = *root;
        while (*x != NULL) {
            bstTimeComplexity++;
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

void insertionSort(int *number) {
    int i, j;
    int insertNode, curr, score;
    for (i = 1; i < size; i++) {
        insertNode = i;
        curr = 0;

        while (curr != insertNode) {
            if (number[curr] > number[insertNode]) {
                score = number[insertNode];
                for (j = 0; j < (insertNode - curr); j++) {
                    bstTimeComplexity++;
                    number[insertNode - j] = number[insertNode - j - 1];
                }
                number[curr] = score;
                break;
            }
            curr++;
        }
    }
}

void SortedArrayTreeInsert(int *number, bsts **root2) {
    *root2 = sortedArrayToBST(number, 0, size - 1);
}

bsts *sortedArrayToBST(int *number, int p, int r) {
    bsts *x, *y;
    int q;
    if (p <= r) {
        bstTimeComplexity++;
        q = p + (r - p) / 2;
        x = calloc(1, sizeof(bsts));
        x->key = number[q];
        x->left = sortedArrayToBST(number, p, q - 1);
        x->right = sortedArrayToBST(number, q + 1, r);
        y = x;
        if (x->left != NULL) {
            x->left->p = y;
        }
        if (x->right != NULL) {
            x->right->p = y;
        }
        return x;
    } else {
        return NULL;
    }
}

void InorderTreeWalk(bsts *root) {
    if (root != NULL) {
        InorderTreeWalk(root->left);
        printf("%d\t", root->key);
        InorderTreeWalk(root->right);
    }
}