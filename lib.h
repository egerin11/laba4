#ifndef LABA4_LIB_H
#define LABA4_LIB_H

#include<stdio.h>
#include <time.h>
typedef struct Tree Tree;
struct Tree {
    int id;
    char *question;
    Tree *left;
    Tree *right;
};

void getString(char **string);

Tree *createBranch(char *question, int id);

char **memory(int capacity);

void addMemory(char ***array, char **answer, int *capacity, int *size);

void findAnswer(Tree *root, char ***answer, int *capacity, int *size);

void inputAnswer(char **answer);

void addBranch(Tree *tree, const char *question, const char *answer);

void writeDataInFile(Tree *root, FILE *file);


Tree *parse(const char *fileName);

void saveTreeData(Tree *root);

void insertNode(Tree **root, int id, char *question, int idYes, int idNo);

void freeTree(Tree *root);

void writeDataForLog(char **userName,char ***answer, int size,time_t t,time_t t2);

void print(Tree *root);

#endif
