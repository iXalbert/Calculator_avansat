#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 100
#define hist_size 100
#define WIN_W 600
#define WIN_H 800

int afterEquals = 0;

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
    return (strcmp(c,"+") == 0 || strcmp(c,"-") == 0 || strcmp(c,"*") == 0 || strcmp(c,"/") == 0);
}

int isFuction(const char *c) {
    return (strcmp(c,"sin") == 0 || strcmp(c,"cos") == 0 || strcmp(c,"sqrt") == 0 || strcmp(c,"log") == 0 || strcmp(c,"CtoF") == 0 || 
                strcmp(c,"FtoC") == 0 || strcmp(c,"DEG") == 0 || strcmp(c,"RAD") == 0 || strcmp(c,"BIN") == 0 || strcmp(c,"HEX") == 0);
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
    
    if(isFuction(op))
        return 3;

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

        else if(infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/'){
            char current_op[2] = {infix[i], '\0'};
            
            while(!isEmptyChar(&s) && strcmp(peekChar(&s), "(") != 0 && 
                  precedente(peekChar(&s)) >= precedente(current_op)){
                strcpy(postfix[(*postfixSize)++], popChar(&s));
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
            }
        }else if(isFuction(postfix[i])){
            double a = popDouble(&s);

            if(strcmp(postfix[i],"sin") == 0)
                pushDouble(&s,sin(a));
            else if(strcmp(postfix[i],"cos") == 0)
                pushDouble(&s,cos(a));
            else if(strcmp(postfix[i],"sqrt") == 0)
                pushDouble(&s,sqrt(a));
            else if(strcmp(postfix[i],"log") == 0)
                pushDouble(&s,log(a));
            else if(strcmp(postfix[i],"CtoF") == 0)
                pushDouble(&s,a* 9.0/5.0 + 32);
            else if(strcmp(postfix[i],"FtoC") == 0)
                pushDouble(&s,(a-32) * 5.0/9.0);
            else if(strcmp(postfix[i],"DEG") == 0)
                pushDouble(&s, a * M_PI / 180.0);
            else if(strcmp(postfix[i],"RAD") == 0)
                pushDouble(&s, a * 180.0 / M_PI);
            else if(strcmp(postfix[i],"BIN") == 0){
                printf("= ");
                printBinary((int)a);
                printf("\n");
                *special_print = 1;
            }
            else if(strcmp(postfix[i],"HEX") == 0){
                printf("= ");
                printHex((int)a);
                printf("\n");
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

typedef struct 
{
    SDL_Rect r;
    char label[32];
}Button;

void drawRect(SDL_Renderer *ren, SDL_Rect *r){
    SDL_RenderFillRect(ren, r);
}

SDL_Texture *renderText(SDL_Renderer *ren, TTF_Font *font, const char *text){
    
    if(!font || !text) return NULL;
    
    SDL_Color color = {255,255,255,255};
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font,text,color);
    if(!surf) return NULL;
    
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren,surf);
    SDL_FreeSurface(surf);
    return tex;
}

int pointInRect(int x, int y, SDL_Rect *r){

    return (x>= r->x && x <= r->x + r->w && y >= r->y && y <= r->y + r->h);
}

#define BTN_ROWS 6
#define BTN_COLS 4

void setUPButtons(Button btns[], int *btnCount){

    int margin = 8;
    int buttonAreaW = WIN_W * 0.6; 
    int btnW = (buttonAreaW - (margin * (BTN_COLS + 1))) / BTN_COLS;
    int btnH = 56;

    int startY = 240;

    const char *labels[BTN_ROWS * BTN_COLS] = {
        "7","8","9","/",
        "4","5","6","*",
        "1","2","3","-",
        "0",".","=","+",
        "sin","cos","sqrt","log",
        "C","DEL","(",")"
    };

    int idx = 0;
    for(int r=0;r<BTN_ROWS && idx < BTN_ROWS * BTN_COLS;r++){
        for(int c=0;c<BTN_COLS && idx < BTN_ROWS * BTN_COLS;c++){
            Button *b = &btns[idx];
            int buttonsLeftX = margin; 
            b->r.x = buttonsLeftX + c * (btnW + margin);
            b->r.y = startY + r * (btnH + margin);
            b->r.w = btnW;
            b->r.h = btnH;
            strncpy(b->label,labels[idx],sizeof(b->label)-1);
            b->label[sizeof(b->label)-1] = '\0';
            idx++; 
        }
    }

    *btnCount = idx;
}

void appendExpr(char *expr, const char *token, char *resultatSTR){

    if(strlen(expr) + strlen(token) + 1 >= MAX) 
        return;


    if(strlen(expr) + strlen(token) < MAX-1){
    if(afterEquals){  
        
        expr[0] = '\0';
        resultatSTR[0] = '\0';
        afterEquals = 0;
    }

    strcat(expr,token);
}

}

void delLast(char *expr){

    int n = strlen(expr);
    if(n>0)
        expr[n-1] = '\0';
}

char historyExpr[hist_size][MAX];
char historyResult[hist_size][MAX];
int histCount = 0;

double memoryValue = 0.0;
int memorySet = 0;

void addHistory(const char *expr, const char *resStr){

    if(histCount < hist_size){

        strcpy(historyExpr[histCount], expr);
        strcpy(historyResult[histCount], resStr);
        histCount++;
    }else{
        for(int i=1;i<hist_size;i++){

         strcpy(historyExpr[i-1], historyExpr[i]);
         strcpy(historyResult[i-1], historyResult[i]); 
        }
        strcpy(historyExpr[hist_size-1], expr);
        strcpy(historyResult[hist_size-1], resStr);  
    }
}

int main(int argc, char **argv){

    if(SDL_Init(SDL_INIT_VIDEO) != 0){

        fprintf(stderr, "SDL_Init error : %s \n", SDL_GetError());
        return 1;
    }

    if(TTF_Init() != 0){

        fprintf(stderr, "TTF_init error : %s \n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Calculaor Avansat", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H,0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    TTF_Font *font = TTF_OpenFont("DejaVuSans.ttf", 18);

    if(!font){
        fprintf(stderr, "Nu a fost gasit acest font\n");
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",18);
        if(!font) {

            font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 18);
            if(!font) {
                fprintf(stderr, "Nu s-a putut deschide fontul\n");
            }
        }
    }

    char expr[MAX] = "";
    char rezultSTR[MAX] = "";
    char postfix[MAX][MAX];
    int postfixSize = 0;

    Button btns[BTN_ROWS * BTN_COLS];
    int btnCnt = 0;
    setUPButtons(btns,&btnCnt);

    int running = 1;
    SDL_Event ev;

    while(running){

        while(SDL_PollEvent(&ev)){

            if(ev.type == SDL_QUIT)
                running = 0;
            else if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT){

                int mx = ev.button.x;
                int my = ev.button.y;

                for(int i=0;i<btnCnt;i++){
                    if(pointInRect(mx,my,&btns[i].r)){

                        const char *lbl = btns[i].label;

                        if(strcmp(lbl,"=") == 0){

                            infixToPosifix(expr,postfix,&postfixSize);
                            int special = 0;
                            char specialBuf[MAX] = "";
                            double res = evaluatePostfix(postfix,postfixSize, &special);
                            if(special){
                                snprintf(rezultSTR, sizeof(rezultSTR), "%s", specialBuf);
                            }else{
                                snprintf(rezultSTR, sizeof(rezultSTR), "%g", res);
                            }

                            addHistory(expr, rezultSTR);

                            expr[0] = '\0';

                            afterEquals = 1;

                        }else if (strcmp(lbl,"C") == 0){
                            expr[0] = '\0';
                            rezultSTR[0] = '\0';
                        }else if (strcmp(lbl,"DEL") == 0){
                            delLast(expr);
                        }else {
                            if(strcmp(lbl, "sin") == 0 || strcmp(lbl,"cos") == 0 || strcmp(lbl, "sqrt") == 0 || strcmp(lbl, "log") == 0){
                                char tmp[MAX];
                                snprintf(tmp,sizeof(tmp), "%s(", lbl);
                                appendExpr(expr,tmp, rezultSTR);
                            }else{
                                char t[32];
                                int p=0;

                                for(int k=0;k<(int)strlen(lbl);k++)
                                    if(!isspace((unsigned char)lbl[k])) t[p++] = lbl[k];
                                    t[p] = '\0';
                                    appendExpr(expr,t, rezultSTR);
                            }
                        }
                    }
                }
            }else if (ev.type == SDL_KEYDOWN){
                SDL_Keycode kc = ev.key.keysym.sym;
                if(kc == SDLK_BACKSPACE) 
                    delLast(expr);
                else if(kc == SDLK_RETURN || kc == SDLK_KP_ENTER){
                    
                    infixToPosifix(expr,postfix,&postfixSize);
                    int special = 0 ;
                    char specialBuf[MAX] = "";
                    double res = evaluatePostfix(postfix,postfixSize, &special);
                    if(special)
                        snprintf(rezultSTR,sizeof(rezultSTR), "%s", specialBuf);
                    else
                        snprintf(rezultSTR,sizeof(rezultSTR), "%g", res);
                    addHistory(expr,rezultSTR);

                    expr[0] = '\0';

                    afterEquals = 1;

                }else if(kc == SDLK_ESCAPE)
                    running = 0;
                else{

                    char ch = (char)ev.key.keysym.sym;
                    if(isprint((unsigned char) ch ) && strlen(expr) < MAX - 2){

                        int len = strlen(expr);
                        expr[len] = ch;
                        expr[len + 1] = '\0';
                    }
                }
            }
        }
        
        SDL_SetRenderDrawColor(ren,20,20,20,255);
        SDL_RenderClear(ren);

        
        SDL_Rect screenRect = {16,16, (int)(WIN_W * 0.6) - 32 , 180};
        SDL_SetRenderDrawColor(ren,40,40,40,255);
        SDL_RenderFillRect(ren,&screenRect);

        
        int histPanelX = (int)(WIN_W * 0.6) + 8;
        int histPanelW = WIN_W - histPanelX - 16;
        SDL_Rect histPanel = {histPanelX, 16, histPanelW, WIN_H - 32};
        SDL_SetRenderDrawColor(ren,35,35,35,255);
        SDL_RenderFillRect(ren, &histPanel);
        SDL_SetRenderDrawColor(ren,80,80,80,255);
        SDL_RenderDrawRect(ren, &histPanel);

        if(font){

            char exprRender[MAX * 2];
            snprintf(exprRender, sizeof(exprRender), "%s", expr);

            SDL_Texture *texExpr = renderText(ren,font,exprRender);

            if(texExpr){

                int tw,th;
                SDL_QueryTexture(texExpr, NULL, NULL, &tw, &th);
                SDL_Rect dst = {screenRect.x + 8, screenRect.y + 8, tw, th};
                SDL_RenderCopy(ren, texExpr, NULL, &dst);
                SDL_DestroyTexture(texExpr);
            }

            SDL_Texture *tesRes = renderText(ren,font,rezultSTR);

            if(tesRes){

                int tw,th;
                SDL_QueryTexture(tesRes,NULL,NULL,&tw,&th);
                SDL_Rect dst = {screenRect.x + 8, screenRect.y + 100 , tw, th};
                SDL_RenderCopy(ren, tesRes, NULL, &dst);
                SDL_DestroyTexture(tesRes);
            }
        }

        for(int i=0;i<btnCnt;i++){

            SDL_Rect r = btns[i].r;
            SDL_SetRenderDrawColor(ren,80,80,80,255);
            SDL_RenderFillRect(ren,&r);

            SDL_SetRenderDrawColor(ren,120,120,120,255);
            SDL_RenderDrawRect(ren,&r);

            if(font){

                SDL_Texture *t = renderText(ren,font,btns[i].label);

                if(t){

                    int tw,th;

                    SDL_QueryTexture(t,NULL,NULL,&tw,&th);
                    SDL_Rect dst = {r.x + (r.w-tw)/2, r.y + (r.h - th)/2, tw, th};
                    SDL_RenderCopy(ren,t,NULL,&dst);
                    SDL_DestroyTexture(t);
                }
            }
        }
        
        
        if(font) {
            int hx = histPanelX + 8;
            int hy = 24;

            SDL_Texture *hTitle = renderText(ren,font,"History:");
            if(hTitle){
                int tw,th;
                SDL_QueryTexture(hTitle,NULL,NULL,&tw,&th);
                SDL_Rect d = {hx,hy,tw,th};
                SDL_RenderCopy(ren,hTitle,NULL,&d);
                SDL_DestroyTexture(hTitle);

                int lineY = hy + 25;
                int maxHistoryItems = (WIN_H - 80) / 20;

                
                int startIdx = (histCount > maxHistoryItems) ? histCount - maxHistoryItems : 0;
                
                for(int i=startIdx; i<histCount && i<startIdx + maxHistoryItems; i++){
                    char line[256];
                    snprintf(line,sizeof(line), "%s = %s", historyExpr[i],historyResult[i]);

                    SDL_Texture *t = renderText(ren,font,line);

                    if(t){
                        int tw,th;
                        SDL_QueryTexture(t,NULL,NULL,&tw,&th);
                        
                        
                        int maxW = histPanelW - 16;
                        if(tw > maxW) tw = maxW;
                        
                        SDL_Rect d = {hx,lineY, tw, th};
                        SDL_RenderCopy(ren,t,NULL,&d);
                        SDL_DestroyTexture(t);
                    }

                    lineY += 20;
                }
            }
        }

        SDL_RenderPresent(ren);
        
        SDL_Delay(16);
    }
    
    if(font)
        TTF_CloseFont(font);
    
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();

    return 0;
    
}
