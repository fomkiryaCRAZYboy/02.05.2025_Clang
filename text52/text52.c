/*
Text 52:
Дан текстовый файл, содержащий таблицу из трех столбцов целых чисел. 
В начале и в конце каждой строки таблицы, а также между ее столбцами 
располагается символ-разделитель. Ширина столбцов таблицы, способ их 
выравнивания и вид символа-разделителя являются произвольными. Создать 
файл целых чисел, содержащий сумму чисел из каждой строки исходной таблицы. 


Выполнил: Фоминых Кирилл Дмитриевич
02.05.2025
Время выполнения: 15 мин
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(void)
{
    FILE* fp = fopen("file_text52.txt", "r");
    if(!fp) {
        perror("file_text52.txt");
        return 1;
    }

    FILE* fd = fopen("res_file.txt", "w");
    if(!fd) {
        fclose(fp);
        perror("res_file.txt");
        return 1;
    }

    char buff[100];
    while(fgets(buff, sizeof(buff), fp)) {
        int num1, num2, num3;
        char* ptr = buff;
        
        while(*ptr && !isdigit(*ptr) && *ptr != '-') ptr++;
        if(*ptr == '\0') continue; 
        num1 = atoi(ptr);
        
        while(*ptr && (isdigit(*ptr) || *ptr == '-')) ptr++;
        while(*ptr && !isdigit(*ptr) && *ptr != '-') ptr++;
        if(*ptr == '\0') continue; 
        num2 = atoi(ptr);
        
        while(*ptr && (isdigit(*ptr) || *ptr == '-')) ptr++;
        while(*ptr && !isdigit(*ptr) && *ptr != '-') ptr++;
        if(*ptr == '\0') continue; 
        num3 = atoi(ptr);
        
        int sum = num1 + num2 + num3;
        fprintf(fd, "%d\n", sum);
    }

    fclose(fp);
    fclose(fd);
    return 0;
}