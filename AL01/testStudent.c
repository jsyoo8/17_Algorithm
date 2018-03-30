#include <stdio.h>
#include <stdlib.h>

typedef struct _student {
    int studentNumber;
    int score;
    struct _student *PREV;
    struct _student *NEXT;
} student;

void insert(int fStudentNumber, int fScore, student *tail);
void insertSortScore(student *head, int size);
void insertSortStudentNumber(student *head, int size);
void printAll(student *head, student *tail);

//파일 실행 옵션으로 txt파일 이름 입력 ex) ./testStudent.exe test1.txt
int main(int argc, char *argv[]) {
    FILE *file;
    char *filename = argv[1];
    int fScore;
    int fStudentNumber;
    int size = 0;
    student *head = malloc(sizeof(student));
    student *tail = malloc(sizeof(student));

    if (argc == 1) {
        fputs("옵션을 입력해 주시기 바랍니다.\n", stderr);
        exit(1);
    }

    if ((file = fopen(filename, "r")) == NULL) {
        printf("파일이 열리지 않습니다.\n");
        exit(1);
    }

    head->NEXT = tail;
    tail->PREV = head;

    while (feof(file) == 0) {
        fscanf(file, "%d %d ", &fStudentNumber, &fScore);
        insert(fStudentNumber, fScore, tail);
        size++;
    }

    // 점수 기준 오름차순
    insertSortScore(head, size);
    printAll(head, tail);
    printf("\n");

    // 학번 기준 오름차순
    insertSortStudentNumber(head, size);
    printAll(head, tail);

    fclose(file);
    printf("완료\n");
    free(head);
    free(tail);
    return 0;
}

void insert(int fStudentNumber, int fScore, student *tail) {
    student *insertNode = malloc(sizeof(student));
    insertNode->studentNumber = fStudentNumber;
    insertNode->score = fScore;

    insertNode->PREV = tail->PREV;
    insertNode->NEXT = tail;
    tail->PREV->NEXT = insertNode;
    tail->PREV = insertNode;
}

void insertSortScore(student *head, int size) {
    student *insertNode;
    int i, j;
    for (i = 1; i < size; i++) {
        insertNode = head->NEXT;
        for (j = 0; j < i; j++) {
            insertNode = insertNode->NEXT;
        }
        student *curr = head->NEXT;

        while (curr != insertNode) {
            if (curr->score > insertNode->score) {
                insertNode->PREV->NEXT = insertNode->NEXT;
                insertNode->NEXT->PREV = insertNode->PREV;
                insertNode->NEXT = curr;
                insertNode->PREV = curr->PREV;
                curr->PREV->NEXT = insertNode;
                curr->PREV = insertNode;
                break;
            }
            curr = curr->NEXT;
        }
    }
}

void insertSortStudentNumber(student *head, int size) {
    student *insertNode;
    int i, j;
    for (i = 1; i < size; i++) {
        insertNode = head->NEXT;
        for (j = 0; j < i; j++) {
            insertNode = insertNode->NEXT;
        }
        student *curr = head->NEXT;

        while (curr != insertNode) {
            if (curr->studentNumber > insertNode->studentNumber) {
                insertNode->PREV->NEXT = insertNode->NEXT;
                insertNode->NEXT->PREV = insertNode->PREV;
                insertNode->NEXT = curr;
                insertNode->PREV = curr->PREV;
                curr->PREV->NEXT = insertNode;
                curr->PREV = insertNode;
                break;
            }
            curr = curr->NEXT;
        }
    }
}

void printAll(student *head, student *tail) {
    student *curr = head->NEXT;

    while (curr != tail) {
        printf("%d %d\n", curr->studentNumber, curr->score);
        curr = curr->NEXT;
    }
}