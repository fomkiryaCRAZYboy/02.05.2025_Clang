/*
    String8. 
    Дано целое число N (> 0) и символ C. Вывести строку длины N, которая состоит из символов C. 
*/

#include <stdio.h>
#include <string.h>

#define N 10

int main(void)
{   
    char stroka[N];
    char sym;
    int i;

    scanf("%c", &sym);

    for(i = 0; i < N-1; ++i)
    {
        stroka[i] = sym;
    }

    stroka[i] = '\0';

    puts(stroka);
    printf("symbols_count = %d", strlen(stroka));

    return 0;
}