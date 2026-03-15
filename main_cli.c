#include "calc_core.h"
#include <stdio.h>
#include <string.h>

int main() {
    calc_init();
    char input[MAX];
    int special_print = 0;

    printf("Calculator avansat (CLI)\n");
    printf("Comenzi: history, M+, MR, MC, quit\n");

    while (1) {
        printf("> ");
        fgets(input, MAX, stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0) break;

        if (strcmp(input, "history") == 0) {
            int count;
            HistoryEntry* hist = calc_get_history(&count);
            for (int i = 0; i < count; i++) {
                printf("%d) %s = %lf\n", i+1, hist[i].expression, hist[i].result);
            }
            continue;
        }

        if (strcmp(input, "MR") == 0) {
            int ok;
            double val = calc_memory_recall(&ok);
            if (ok) printf("Memorie = %lf\n", val);
            else printf("Memorie goala\n");
            continue;
        }

        if (strcmp(input, "MC") == 0) {
            calc_memory_clear();
            printf("Memoria stearsa\n");
            continue;
        }

        if (strncmp(input, "M+", 2) == 0) {
            double val = calc_evaluate(input+2, &special_print);
            calc_memory_add(val);
            printf("Adaugat %lf in memorie\n", val);
            continue;
        }

        double rezultat = calc_evaluate(input, &special_print);
        if (!special_print) {
            printf("Rezultat = %lf\n", rezultat);
        }
    }
    return 0;
}
