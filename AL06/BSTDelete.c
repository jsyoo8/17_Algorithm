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

bsts *TreeMinimum(bsts *root);

bsts *TreeMaximum(bsts *root);

bsts *TreeSuccessor(bsts *root);

bsts *TreePredecessor(bsts *root);

bsts *TreeSearch(bsts **root, int k);

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
    int deleteNumber = 3;
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

    printAll(root, timeComplexity);

    printf("삭제 %d\n", deleteNumber);

    startTime = clock();
    TreeDelete(&root, deleteNumber);
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
    printf("\n실행 시간 : %f(ms) \tbst : %d \n", timeComplexity, bstTimeComplexity);
}

void InorderTreeWalk(bsts *root) {
    if (root != NULL) {
        InorderTreeWalk(root->left);
        printf("%d\t", root->key);
        InorderTreeWalk(root->right);
    }
}

bsts *TreeMinimum(bsts *root) {
    bsts **x;
    x = &root;
    while ((*x)->left != NULL) {
        x = &((*x)->left);
    }
    return *x;
}

bsts *TreeMaximum(bsts *root) {
    bsts **x;
    x = &root;
    while ((*x)->right != NULL) {
        x = &((*x)->right);
    }
    return *x;
}

bsts *TreeSuccessor(bsts *root) {
    bsts **x, **y;
    x = &root;
    if ((*x)->right != NULL) {
        return TreeMinimum((*x)->right);
    }
    y = &((*x)->p);
    while ((*y) != NULL && *x == (*y)->right) {
        x = y;
        y = &((*y)->p);
    }
    return *y;
}

bsts *TreePredecessor(bsts *root) {
    bsts **x, **y;
    x = &root;
    if ((*x)->left != NULL) {
        return TreeMaximum((*x)->left);
    }
    y = &((*x)->p);
    while ((*y) != NULL && *x == (*y)->left) {
        x = y;
        y = &((*y)->p);
    }
    return *y;
}

bsts *TreeSearch(bsts **root, int k) {
    bsts **x;
    x = root;
    while (*x != NULL && k != (*x)->key) {
        if (k < (*x)->key) {
            x = &((*x)->left);
        } else {
            x = &((*x)->right);
        }
    }
    return *x;
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

void TreeDelete(bsts **root, int i) {
    bsts *y, *z;
    z = TreeSearch(root, i);
    if (z != NULL) {
        if (z->left == NULL) {
            Transplant(root, z, z->right);
        } else if (z->right == NULL) {
            Transplant(root, z, z->left);
        } else {
            y = TreeMinimum(z->right);
            if (y->p != z) {
                Transplant(root, y, y->right);
                y->right = z->right;
                if (y->right != NULL) {
                    y->right->p = y;
                }
            }
            Transplant(root, z, y);
            y->left = z->left;
            if (y->left != NULL) {
                y->left->p = y;
            }
        }
    }
}

void Transplant(bsts **root, bsts *u, bsts *v) {
    if (u->p == NULL) {
        *root = v;
    } else if (u == u->p->left) {
        (*(u->p)).left = v;
    } else {
        (*(u->p)).right = v;
    }
    if (v != NULL) {
        v->p = u->p;
    }
}