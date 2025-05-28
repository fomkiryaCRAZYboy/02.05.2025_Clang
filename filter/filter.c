/*
    Программа принимает два аргумента: путь к файлу и ключевое слово-фильтр.
    Создается файл, в который записываются все строки, содержащие фильтр и их номера.
    Выводится количество вхождений фильтра в файл. 
*/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define BUF_SIZE    4096


int main(int argc, char* argv[])
{
    if(argc != 3)
    {   
        fprintf(stderr, "ERROR\nUsage: %s [path_to_file] [filter]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* fd = fopen(argv[1], "r");
    if(!fd)
    {
        perror("failed fopen(3)");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    int str_num = 1;
    int entry_count = 0;
    while(fgets(buf, BUF_SIZE, fd))
    {
        if(strstr(buf, argv[2]))
        {
            fprintf(stdout, "%d. %s", str_num, buf);
            ++entry_count;
        }
        ++str_num;

    }
    fprintf(stdout, "\nentry count = %d\n", entry_count);

    fclose(fd);
    exit(EXIT_SUCCESS);
}