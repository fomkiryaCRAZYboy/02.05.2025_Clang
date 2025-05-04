/*
Text44:
Дан текстовый файл, каждая строка которого изображает целое число, 
дополненное слева и справа несколькими пробелами. Вывести количество этих чисел и их сумму. 

Выполнил: Фоминых Кирилл Дмитриевич
02.05.2025
Время выполнения: 15 мин
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum { max_buf_size = 100 };

int main(void)
{
    FILE* file = fopen("file_text44.txt", "r");
    if(!file) {
        perror("file_text44.txt");
        return 1;
    }

    char buffer[max_buf_size];
    int sum = 0;
    int count = 0;

    while(!feof(file)){
        fgets(buffer, sizeof(buffer), file);
        char* ptr = buffer;

        while(isspace(*ptr)) ptr++;

        if(*ptr == '\0') break;
 
        long num = 0;
        if(sscanf(buffer, "%ld", &num));

        count++;
        sum += num;
    }

    fprintf(stdout, "sum = %d, count = %d", sum, count);
    
    fclose(file);

    return 0;
}