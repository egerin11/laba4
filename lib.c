#include<stdlib.h>
#include <string.h>
#include"lib.h"
#include <stdio.h>
#include<stdbool.h>
#include<time.h>

void getString(char **string) {
    int size = 0;
    int realSize = 8;
    *string = (char *) malloc(realSize * sizeof(char));
    rewind(stdin);
    char check;
    while (true) {
        check = (char) getchar();
        if (check == '\n') {
            if (size + 1 < realSize) {
                *string = (char *) realloc(*string, (size + 1) * sizeof(char));
            }
            (*string)[size] = '\0';
            return;
        }

        (*string)[size] = check;
        size++;
        if (size + 1 == realSize) {
            realSize *= 2;
            *string = (char *) realloc(*string, realSize * sizeof(char));
        }
    }
}


Tree *createBranch(char *question, const int id) {
    Tree *tree = (Tree *) malloc(sizeof(Tree));
    tree->id = id;
    tree->question = strdup(question);
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

//void addBranch(Tree *tree, const char *question, const char *answer) {
//    if (strcmp(answer, "yes") == 0) {
//        if (tree->right == NULL) tree->right = createBranch(question);
//        else addBranch(tree, question, answer);
//    } else if (strcmp(answer, "no") == 0) {
//        if (tree->left == NULL) tree->left = createBranch(question);
//        else addBranch(tree, question, answer);
//
//    }
//}


char **memory(const int capacity) {
    char **array = (char **) malloc(capacity * sizeof(char *));
    return array;
}

void addMemory(char ***array, char **answer, int *capacity, int *size) {
    if (*size + 1 == *capacity) {
        *capacity *= 2;
        *array = (char **) realloc(*array, *capacity * sizeof(char *));
    }
    (*array)[*size] = *answer;
    (*size)++;
}

void inputAnswer(char **answer) {
    while (strcmp(*answer, "yes") != 0 &&
           strcmp(*answer, "no") != 0) {
        getString(answer);
    }
}

void findAnswer(Tree *root, char ***answer, int *capacity, int *size) {
    if (root->right == NULL) printf("%s ?", root->question);
    else printf("%s ", root->question);
    char *array = malloc(1);
    inputAnswer(&array);
    addMemory(answer, &array, capacity, size);
    if (strcmp(array, "yes") == 0) {
        if (root->right != NULL) {
            addMemory(answer, &(root->question), capacity, size);
            findAnswer(root->right, answer, capacity, size);
        } else {
            printf("Тот кто писал этот код явно ввел этот ответ!!!!\n");
            free(array);
        }
    } else if (strcmp(array, "no") == 0) {
        if (root->left != NULL) {
            addMemory(answer, &(root->question), capacity, size);
            findAnswer(root->left, answer, capacity, size);
        } else {
            printf("Видимо в моей базе нет такого объекта.\nCейчас победили вы человечки,но настанет мой час!!!!\nВведите что это было\n");
            char *newQuestion;
            char *newObject;
            getString(&newObject);
            addMemory(answer, &newObject, capacity, size);
            printf("Напишите как отличить это %s от этого %s\n", newObject, root->question);
            getString(&newQuestion);
            addMemory(answer, &newQuestion, capacity, size);
            Tree *newQuestionRoot = createBranch(newQuestion, *size);
            Tree *newObjectRoot = createBranch(newObject, *size + 1);
            newQuestionRoot->right = newObjectRoot;
            root->left = newQuestionRoot;

        }
    }
}

void writeDataInFile(Tree *root, FILE *file) {
    if (root != NULL) {
        fprintf(file, "\"id\": %d\n", root->id);
        fprintf(file, "\"question\": \"%s\"\n", root->question);
        if (root->right != NULL) {
            fprintf(file, "\"yes\": %d\n", root->right->id);
        } else fprintf(file, "\"yes\": %d\n", root->id - 1);
        if (root->left != NULL) {
            fprintf(file, "\"no\": %d\n", root->left->id);
        } else fprintf(file, "\"no\": %d\n", root->id);
        writeDataInFile(root->right, file);
        writeDataInFile(root->left, file);
    }
}

void saveTreeData(Tree *root) {
    FILE *file = fopen("data.txt", "w");
    if (file != NULL) {
        writeDataInFile(root, file);
        fclose(file);
    } else {
        printf("Error: could not open file\n");
    }
}


void insertNode(Tree **root, int id, char *question, int idYes, int idNo) {
    Tree *node = createBranch(question, id);
    if (*root == NULL) {
        *root = node;
    } else {
        Tree *temp = *root;
        while (temp != NULL) {
            if (idYes != 0 && idYes == temp->id) {
                node->left = temp->left;
                temp->left = node;
                break;
            } else if (idNo != 0 && idNo == temp->id) {
                node->right = temp->right;
                temp->right = node;
                break;
            } else if (temp->left == NULL) {
                temp->left = node;
                break;
            } else if (temp->right == NULL) {
                temp->right = node;
                break;
            } else {
                temp = temp->left;
            }
        }
    }
}

Tree *parse(const char *fileName) {
    Tree *root = NULL;
    FILE *file = fopen(fileName, "r");
    int id;
    int idYes;
    int idNo;
    char question[256];
    if (file != NULL) {
        while (fscanf(file, "\"id\": %d\n\"question\": \"%[^\"]\"\n\"yes\": %d\n\"no\": %d\n", &id, question, &idYes,
                      &idNo) != EOF) {
            insertNode(&root, id, question, idYes, idNo);
        }
    }
    fclose(file);
    return root;
}


void print(Tree *root) {
    if (root != NULL) {
        printf("id: %d\n", root->id);
        printf("question: %s\n", root->question);
        if (root->right != NULL) {
            printf("yes: %d\n", root->right->id);
        }
        if (root->left != NULL) {
            printf("no: %d\n", root->left->id);
        }
        printf("\n");
        print(root->right);
        print(root->left);
    }
}


void writeDataForLog(char **userName, char ***answer, const int size, time_t t, time_t t2) {
    FILE *file = fopen("log.txt", "a");
    struct tm *tm = localtime(&t);
    struct tm *tm2 = localtime(&t2);
    char str[1000];
    char str2[1000];
    if (file != NULL) {
        fprintf(file, "user name %s\n", *userName);
        strftime(str, sizeof(str), "%d.%m.%Y %H:%M:%S", tm);
        fprintf(file, "begin %s\n", str);
        strftime(str2, sizeof(str2), "%d.%m.%Y %H:%M:%S", tm2);
        fprintf(file, "end %s\n", str2);
        for (int i = 0; i < size; i++) {
            fprintf(file, "%s ", (*answer)[i]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}


void freeTree(Tree *root) {
    if (root != NULL) {
        freeTree(root->right);
        freeTree(root->left);
        free(root->question);
        free(root);
    }
}