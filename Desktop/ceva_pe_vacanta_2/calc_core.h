#ifndef CALC_CORE_H
#define CALC_CORE_H

#include <stdio.h>

#define MAX 100
#define MEMORY_SIZE 100

typedef struct {

    char expression[MAX];
    double result;
} HistoryEntry;

void calc_init();

double calc_evaluate(const char *expr, int *special_print);

HistoryEntry* calc_get_history(int *count);

void calc_memory_add(double val);
double calc_memory_recall(int *ok);
void calc_memory_clear();

//void infixToPostfix(const char *infix, char postfix[MAX][MAX], int *postfixSize);
//double evaluatePostfix(char postfix[MAX][MAX], int postfixSize, int *special_print);

#endif
