#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 100
#define memory_size 100

int angle_mode = 0;

typedef struct {

    char data[MAX][MAX];
    int top;
}CharStack;

void initCharStack(CharStack *s){

    s->top = -1;
}

int isEmptyChar(CharStack *s){

    return s->top == -1;
}

void pushChar(CharStack *s, const char *str){

    strcpy(s->data[++(s->top)],str);
}

char* popChar(CharStack *s){

    return s->data[(s->top)--];
}

char* peekChar(CharStack *s){

    return s->data[(s->top)];
}

typedef struct{

    double data[MAX];
    int top;
}DoubleStack;

void initDoubleStack(DoubleStack *s){

    s->top = -1;
}

int isEmptyDouble(DoubleStack *s){

    return s->top == -1;
}

void pushDouble(DoubleStack *s, double val){

    s->data[++(s->top)] = val;
}

double popDouble(DoubleStack *s){

    return s->data[(s->top)--];
}

int isOperator(const char *c) {
    return (strcmp(c,"+") == 0 || strcmp(c,"-") == 0 || strcmp(c,"*") == 0 || strcmp(c,"/") == 0 || strcmp(c,"^") == 0);
}

int isFuction(const char *c) {
    return (strcmp(c,"sin") == 0 || strcmp(c,"cos") == 0 || strcmp(c,"sqrt") == 0 || strcmp(c,"log") == 0 || strcmp(c,"CtoF") == 0 || 
                strcmp(c,"FtoC") == 0 || strcmp(c,"DEG") == 0 || strcmp(c,"RAD") == 0 || strcmp(c,"BIN") == 0 || strcmp(c,"HEX") == 0 ||
                strcmp(c,"pow") == 0);
}

int precedente_char(char op){
    if(op == '+' || op == '-')
        return 1;
    if(op == '*' || op == '/')
        return 2;
    return 0;
}

int precedente(const char *op){

    if(strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;

    if(strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
        return 2;
    
    if(strcmp(op, "^") == 0)
        return 3;
    
    if(isFuction(op))
        return 4;

    return 0;
}

void infixToPosifix(const char *infix, char postfix[][MAX], int *postfixSize){

    CharStack s;
    initCharStack(&s);
    *postfixSize = 0;

    int i=0;

    while(infix[i] != '\0'){

        if(isspace(infix[i])){
            i++;
            continue;
        }

        if(isdigit(infix[i]) || (infix[i] == '.' && isdigit(infix[i+1]))){
            char num[MAX];
            int j=0;
            while(isdigit(infix[i]) || infix[i] == '.'){
                num[j++] = infix[i++];
            }
            num[j] = '\0';
            strcpy(postfix[(*postfixSize)++], num);
            continue;
        }

        if(isalpha(infix[i])){
            char func[MAX];
            int j=0;
            while(isalpha(infix[i])){
                func[j++] = infix[i++]; 
            }
            func[j] = '\0';
            pushChar(&s, func);
            continue;
        }

        if(infix[i] == '('){
            char op[2] = {infix[i], '\0'};
            pushChar(&s, op);
        }
        
        else if(infix[i] == ')'){
            while(!isEmptyChar(&s) && strcmp(peekChar(&s), "(") != 0){
                strcpy(postfix[(*postfixSize)++], popChar(&s));
            }
            if(!isEmptyChar(&s) && strcmp(peekChar(&s), "(") == 0){
                popChar(&s);
            }
        }

        else if(infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/' || infix[i] == '^'){
            char current_op[2] = {infix[i], '\0'};
            
            if(infix[i] == '^'){
                while(!isEmptyChar(&s) && strcmp(peekChar(&s), "(") != 0 && 
                      precedente(peekChar(&s)) > precedente(current_op)){
                    strcpy(postfix[(*postfixSize)++], popChar(&s));
                }
            } else {
                while(!isEmptyChar(&s) && strcmp(peekChar(&s), "(") != 0 && 
                      precedente(peekChar(&s)) >= precedente(current_op)){
                    strcpy(postfix[(*postfixSize)++], popChar(&s));
                }
            }
            pushChar(&s, current_op);
        }

        i++;
    }

    while(!isEmptyChar(&s)){
        strcpy(postfix[(*postfixSize)++], popChar(&s));
    }
}

void printBinary(int n){

    if(n==0){
        printf("0");
        return;
    }

    char buff[64];
    int i=0;

    while(n>0){

        buff[i++] = (n%2) + '0';
        n=n/2;
    }

    for(int j=i-1;j>=0;j--){
        printf("%c", buff[j]);
    }

}

void printHex(int n){
    printf("%X", n);
}

double evaluatePostfix(char postfix[][MAX], int size, int *special_print){

    DoubleStack s;
    initDoubleStack(&s);
    *special_print = 0;

    for(int i=0;i<size;i++){

        if(isOperator(postfix[i])){

            double b = popDouble(&s);
            double a = popDouble(&s);

            switch(postfix[i][0]){

                case '+' : pushDouble(&s, a + b); break;
                case '-' : pushDouble(&s, a - b); break;
                case '*' : pushDouble(&s, a * b); break;
                case '/' : pushDouble(&s, a / b); break;
                case '^' : pushDouble(&s, pow(a, b)); break;
            }
        }else if(isFuction(postfix[i])){
            
            if(strcmp(postfix[i],"sin") == 0){
                double a = popDouble(&s);
                
                if(angle_mode == 1) a = a * M_PI / 180.0; 
                pushDouble(&s,sin(a));
            }
            else if(strcmp(postfix[i],"cos") == 0){
                double a = popDouble(&s);
                
                if(angle_mode == 1) a = a * M_PI / 180.0; 
                pushDouble(&s,cos(a));
            }
            else if(strcmp(postfix[i],"sqrt") == 0){
                double a = popDouble(&s);
                pushDouble(&s,sqrt(a));
            }
            else if(strcmp(postfix[i],"log") == 0){
                double a = popDouble(&s);
                pushDouble(&s,log(a));
            }
            else if(strcmp(postfix[i],"CtoF") == 0){
                double a = popDouble(&s);
                pushDouble(&s,a* 9.0/5.0 + 32);
            }
            else if(strcmp(postfix[i],"FtoC") == 0){
                double a = popDouble(&s);
                pushDouble(&s,(a-32) * 5.0/9.0);
            }
            else if(strcmp(postfix[i],"DEG") == 0){
                
                angle_mode = 1;
                pushDouble(&s, 0); 
            }
            else if(strcmp(postfix[i],"RAD") == 0){

                angle_mode = 0;
                pushDouble(&s, 0); 
            }
            else if(strcmp(postfix[i],"pow") == 0){
                double b = popDouble(&s);
                double a = popDouble(&s);
                pushDouble(&s, pow(a, b));
            }
            else if(strcmp(postfix[i],"BIN") == 0){
                double a = popDouble(&s);
                printf("= ");
                printBinary((int)a);
                printf("\n");
                pushDouble(&s, a); 
                *special_print = 1;
            }
            else if(strcmp(postfix[i],"HEX") == 0){
                double a = popDouble(&s);
                printf("= ");
                printHex((int)a);
                printf("\n");
                pushDouble(&s, a); 
                *special_print = 1;
            }
        }else{
            pushDouble(&s, atof(postfix[i]));
        }
    }

    if(isEmptyDouble(&s))
        return 0;
    else
        return popDouble(&s);
}

void replace_ans(const char *in, char *out, double ans){

    char buf_ans[64];
    snprintf(buf_ans,sizeof(buf_ans),"%.15g",ans);

    int i=0;
    int o=0;

    while(in[i] != '\0'){

        if(in[i] && in[i+1] && in[i+2] && 
            tolower((unsigned char) in[i]) == 'a' && 
                tolower((unsigned char) in[i+1]) == 'n' && 
                    tolower((unsigned char) in[i+2]) == 's'){

                        int prev_ans = (i > 0 && (isalnum((unsigned char) in[i-1]) || in[i-1] == '_'));
                        int next_ans = (isalnum((unsigned char) in[i+3]) || in[i+3] == '_');
                        if(!prev_ans && !next_ans){

                            int k=0;
                            while(buf_ans[k])
                                out[o++] = buf_ans[k++];
                            i = i + 3;
                            continue;
                        }
                    }

        out[o++] = in[i++];
    }

    out[o] = '\0';
}

int main(){

    char infix[MAX];
    char tempExpr[MAX];
    char postfix[MAX][MAX];
    int postfixSize;

    int special_print = 0;

    char history[memory_size][MAX];
    double results[memory_size];
    int histCount = 0;

    double memory = 0.0;
    int memory_set = 0;

    double lastResult = 0.0;
    int lastSet = 0;

    printf("Calculator avansat : \n");
    printf("Functii speciale : sin,cos,sqrt,log,pow,CtoF,FtoC,DEG,RAD,BIN,HEX \n");
    printf("Operatori : +, -, *, /, ^ (exponentiere) \n");
    printf("Comanda peciala : history, M+,M-,MR,MC,MS <expresie>, quit \n\n");

    while(1){

        printf("> ");
        
        fgets(infix, MAX, stdin);
        infix[strcspn(infix, "\n")] = '\0';

        if(strcmp(infix,"quit") == 0)
            break;
            
        if(strcmp(infix,"history") == 0){
            for(int i=0;i<histCount;i++){
                printf("%d) %s = %lf \n", i+1,history[i],results[i]);
            }
            continue;
        }

        if(strcmp(infix,"MR") == 0){
            if(memory_set){
                printf("Memorie = %lf \n", memory);
            }else{
                printf("Memorie goala, nu a fost salvat nimic inca \n\n");
            }

            continue;
        }

        if(strcmp(infix,"MC") == 0){
            memory_set = 0;
            printf("Memoria a fost stearsa\n\n");
            continue;
        }

        if(strcmp(infix, "ANS") == 0){

            if(lastSet)
                printf("ANS = %g \n", lastResult);
            else 
                printf("ANS nu e setat inca \n");
            continue;
        }

        if(strncmp(infix,"M+", 2) == 0){

            char expr[MAX];
            strcpy(expr,infix+2);
            infixToPosifix(expr,postfix,&postfixSize);
            double rezultat = evaluatePostfix(postfix,postfixSize,&special_print);
            memory = memory + rezultat;
            memory_set = 1;
            printf("Rezultatul %lf a fost adaugat in memorie. Memorie = %lf \n", rezultat, memory);
            continue;
        }

        if(strncmp(infix,"MS",2) == 0){

            char expr[MAX];
            strcpy(expr,infix+2);
            infixToPosifix(expr,postfix,&postfixSize);
            double rezultat = evaluatePostfix(postfix,postfixSize,&special_print);
            memory = rezultat;
            memory_set = 1;
            printf("Rezultatul %lf a fost salvat in memeorie. Memoria  = %lf \n", rezultat, memory);
            continue;
        }

        if(strncmp(infix,"M-",2) == 0){
            
            char expr[MAX];
            strcpy(expr,infix+2);
            infixToPosifix(expr,postfix,&postfixSize);
            double rezultat = evaluatePostfix(postfix,postfixSize,&special_print);
            memory = memory - rezultat;
            memory_set = 1;
            printf("Rezulataul %lf a fsot scazut din memorie. Memoria = %lf \n", rezultat,memory);
            continue;
        }

        replace_ans(infix,tempExpr,lastResult);

        infixToPosifix(infix,postfix, &postfixSize);

        printf("Expresie in postfix : \n");

        for(int i=0;i<postfixSize;i++){

            printf("%s ", postfix[i]);
        }

        printf("\n");
        
        int special_print = 0;
        double rezultat = evaluatePostfix(postfix,postfixSize, &special_print);

        if(!special_print){
            printf("Rezultatul = %lf \n", rezultat);
            lastResult = rezultat;
            lastSet = 1;
        }

        if(histCount < memory_size){
            strcpy(history[histCount], tempExpr);
            results[histCount] = rezultat;
            histCount++;
        }else{
            for(int i=1;i<memory_size;i++){
                strcpy(history[i-1],history[i]);
                results[i-1] = results[i];
            }
            strcpy(history[memory_size-1], tempExpr);
            results[memory_size-1] = rezultat;
        }
    }

    return 0;
}