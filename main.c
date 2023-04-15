#include <stdio.h>
#include"lib.h"
#include<time.h>

int main() {
    int capacity = 8;
    int size = 0;
    char **answer = memory(capacity);
    char *userName;
    printf("input user name\n");
    getString(&userName);
    time_t t = time(NULL);
    Tree *root = parse("data.txt");
    findAnswer(root, &answer, &capacity, &size);
    saveTreeData(root);
    time_t t2 = time(NULL);
    writeDataForLog(&userName, &answer, size, t, t2);
    for (int i = 0; i < size; i++)
        printf("%s \n", answer[i]);
    printf("----------");
    print(root);
    freeTree(root);
    return 0;
}
