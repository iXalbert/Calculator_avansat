#include "calc_core.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>


static HistoryEntry history[MEMORY_SIZE];
static int histCount = 0;

static double memory = 0.0;
static int memory_set = 0;


static int getPrecedence(const char *op) {
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 2;
    if (strcmp(op, "^") == 0) return 3;
    return 0;
}

static int isOperator(const char *t) {
    return (strcmp(t, "+") == 0 || strcmp(t, "-") == 0 ||
            strcmp(t, "*" ) == 0 || strcmp(t, "/" ) == 0 ||
            strcmp(t, "^") == 0);
}

static int isFunction(const char *t) {
    return (strcmp(t,"sin") == 0 || strcmp(t,"cos") == 0 ||
            strcmp(t,"sqrt") == 0 || strcmp(t,"log") == 0 ||
            strcmp(t,"CtoF") == 0 || strcmp(t,"FtoC") == 0 ||
            strcmp(t,"DEG") == 0 || strcmp(t,"RAD") == 0 ||
            strcmp(t,"BIN") == 0 || strcmp(t,"HEX") == 0);
}


static void toBinary(int n, char *out) {
    if (n == 0) { strcpy(out,"0"); return; }
    char buf[64]; int i=0;
    while (n > 0) { buf[i++] = (n%2) + '0'; n/=2; }
    for (int j=0;j<i;j++) out[j] = buf[i-1-j];
    out[i] = '\0';
}

static void toHex(int n, char *out) {
    sprintf(out,"%X",n);
}


static void infixToPostfix(const char *infix, char postfix[MAX][MAX], int *postfixSize) {
    char stack[MAX][MAX]; 
    int top = -1;
    int i=0, k=0;

    while (infix[i] != '\0') {
        if (isspace(infix[i])) { i++; continue; }

        
        if (isdigit(infix[i]) || (infix[i]=='.' && isdigit(infix[i+1]))) {
            int j=0;
            while (isdigit(infix[i]) || infix[i]=='.') {
                postfix[k][j++] = infix[i++];
            }
            postfix[k][j] = '\0';
            k++;
            continue;
        }

        
        if (isalpha(infix[i])) {
            int j=0; char func[MAX];
            while (isalpha(infix[i])) {
                func[j++] = infix[i++];
            }
            func[j] = '\0';
            strcpy(stack[++top], func);
            continue;
        }

        
        if (infix[i]=='(') {
            strcpy(stack[++top], "(");
        }
        else if (infix[i]==')') {
            while (top>=0 && strcmp(stack[top],"(")!=0) {
                strcpy(postfix[k++], stack[top--]);
            }
            if (top>=0 && strcmp(stack[top],"(")==0) top--; 
            
            if (top>=0 && isFunction(stack[top])) {
                strcpy(postfix[k++], stack[top--]);
            }
        }

        
        else {
            char op[2] = {infix[i], '\0'};
            while (top>=0 && strcmp(stack[top],"(")!=0 &&
                   getPrecedence(stack[top]) >= getPrecedence(op)) {
                strcpy(postfix[k++], stack[top--]);
            }
            strcpy(stack[++top], op);
        }
        i++;
    }

    while (top>=0) {
        strcpy(postfix[k++], stack[top--]);
    }

    *postfixSize = k;
}


static double evaluatePostfix(char postfix[MAX][MAX], int size, int *special_print, char *special_out) {
    double stack[MAX]; int top=-1;
    *special_print = 0;
    special_out[0] = '\0';

    for (int i=0;i<size;i++) {
        char *tok = postfix[i];

        if (isOperator(tok)) {
            double b = stack[top--];
            double a = stack[top--];
            if (strcmp(tok,"+")==0) stack[++top] = a+b;
            else if (strcmp(tok,"-")==0) stack[++top] = a-b;
            else if (strcmp(tok,"*")==0) stack[++top] = a*b;
            else if (strcmp(tok,"/")==0) stack[++top] = a/b;
            else if (strcmp(tok,"^")==0) stack[++top] = pow(a,b);
        }
        else if (isFunction(tok)) {
            double a = stack[top--];
            if (strcmp(tok,"sin")==0) stack[++top] = sin(a);
            else if (strcmp(tok,"cos")==0) stack[++top] = cos(a);
            else if (strcmp(tok,"sqrt")==0) stack[++top] = sqrt(a);
            else if (strcmp(tok,"log")==0) stack[++top] = log(a);
            else if (strcmp(tok,"CtoF")==0) stack[++top] = a*9.0/5.0+32;
            else if (strcmp(tok,"FtoC")==0) stack[++top] = (a-32)*5.0/9.0;
            else if (strcmp(tok,"DEG")==0) stack[++top] = a*M_PI/180.0;
            else if (strcmp(tok,"RAD")==0) stack[++top] = a*180.0/M_PI;
            else if (strcmp(tok,"BIN")==0) { 
                toBinary((int)a,special_out); 
                *special_print=1; 
                stack[++top]=a; 
            }
            else if (strcmp(tok,"HEX")==0) { 
                toHex((int)a,special_out); 
                *special_print=1; 
                stack[++top]=a; 
            }
        }
        else { 
            stack[++top] = atof(tok);
        }
    }

    return (top>=0) ? stack[top] : 0.0;
}


void calc_init() {
    histCount = 0;
    memory = 0.0;
    memory_set = 0;
}

double calc_evaluate(const char *expr, int *special_print) {
    char postfix[MAX][MAX];
    int size;
    char special_buf[MAX];
    infixToPostfix(expr,postfix,&size);

    double rezultat = evaluatePostfix(postfix,size,special_print,special_buf);

    if (histCount < MEMORY_SIZE) {
        strcpy(history[histCount].expression, expr);
        if (*special_print) {
            strcpy(history[histCount].expression, expr);
            strcpy(history[histCount].expression, expr);
        }
        history[histCount].result = rezultat;
        histCount++;
    }
    if (*special_print) {
        printf("Rezultat special = %s\n", special_buf);
    }
    return rezultat;
}

HistoryEntry* calc_get_history(int *count) {
    *count = histCount;
    return history;
}

void calc_memory_add(double val) {
    memory += val;
    memory_set = 1;
}

double calc_memory_recall(int *ok) {
    if (memory_set) { *ok=1; return memory; }
    *ok=0; return 0.0;
}

void calc_memory_clear() { 
    memory_set = 0; 
}
