/*!String70:
Дана строка, содержащая латинские буквы и скобки трех видов: «()», «[]», «{}». 
Если скобки расставлены правильно (т. е. каждой открывающей соответствует закрывающая
скобка того же вида), то вывести число 0. В противном случае вывести или номер позиции,
в которой расположена первая ошибочная скобка, или, если закрывающих скобок не хватает, число −1. 

Выполнил: Фоминых Кирилл Дмитриевич
02.05.2025
Время выполнения: 17 minutes
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRING_LEN  50

typedef enum { round, square, curly, no_bracket = -1 } LAST_OPEN_BRACKET;

int main(void)
{
    char string[MAX_STRING_LEN];
    if(!fgets(string, MAX_STRING_LEN, stdin)) {
        if(feof(stdin)){
            fprintf(stderr, "Достигнут конец ввода\n");
        }
        else {
            perror("Ошибка чтения из stdin\n");
            return 1;
        }
    }

    int round_brackets_lvl = 0;
    int square_brackets_lvl = 0;
    int curly_brackets_lvl = 0;

    LAST_OPEN_BRACKET last_open_bracket = no_bracket;


    for(int i = 0; string[i]; i++){
        if(string[i] == '(') {
            round_brackets_lvl++;
            last_open_bracket = round;
        }
        if(string[i] == ')') {
            if(!round_brackets_lvl){
                printf("Error: Missed '(' (err bracket position: %d)", i+1);
                return 1;
            }
            if((square_brackets_lvl != 0 || curly_brackets_lvl != 0) && last_open_bracket != round) {
                printf("Error: Missed bracket (err position: %d)", i+1);
                return 1;
            }
            round_brackets_lvl--;
        }

        if(string[i] == '[') {
            square_brackets_lvl++;
            last_open_bracket = square;
        }
        if(string[i] == ']') {
            if(!square_brackets_lvl){
                printf("Error: Missed '[' (err bracket position: %d)", i+1);
                return 1;
            }
            if((round_brackets_lvl != 0 || curly_brackets_lvl != 0) && last_open_bracket != square) {
                printf("Error: Missed bracket (err position: %d)", i+1);
                return 1;
            }
            square_brackets_lvl--;
        }

        if(string[i] == '{') {
            curly_brackets_lvl++;
            last_open_bracket = curly;
        }
        if(string[i] == '}') {
            if(!curly_brackets_lvl){
                printf("Error: Missed '{' (err bracket position: %d)", i+1);
                return 1;
            }
            if((square_brackets_lvl != 0 || round_brackets_lvl != 0) && last_open_bracket != curly) {
                printf("Error: Missed bracket (err position: %d)", i+1);
                return 1;
            }
            curly_brackets_lvl--;
        }
    }

    if(round_brackets_lvl || square_brackets_lvl || curly_brackets_lvl) {
        printf("Error: Missed close bracket\n-1");
    }

    return 0;
}