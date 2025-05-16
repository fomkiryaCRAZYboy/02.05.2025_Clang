/* программа находит ppid текущего процесса */

#define _POSIX_C_SOURCE 200809L //современный стандарт POSIX, без которого могут не работать некоторые функции и определения (к примеру, NAME_MAX)

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <ctype.h>

#include <limits.h>

#define MAX_PID_LEN 13


int main(void)
{
    //получаю pid текущего процесса и записываю в файл (текущий процесс - ппроцесс выполнения данной программы)
    pid_t curr_pid = getpid();

    char path_to_status_file[PATH_MAX];
    snprintf(path_to_status_file, PATH_MAX, "/proc/%d/status", curr_pid);

    FILE* status_file = fopen(path_to_status_file, "r");
    if(!status_file){
        fprintf(stderr, "errno %d: %s", errno, strerror(errno));
        return 1;
    }

    char ppid_str[MAX_PID_LEN+10];   //получаю строку: PPid:    [ppid]
    while(fgets(ppid_str, MAX_PID_LEN, status_file)){
        if(strstr(ppid_str, "PPid:")) break;
    }

    char ppid[MAX_PID_LEN];

    int i = 0;
    int j = 0;
    while(ppid_str[i]){
        if(isdigit(ppid_str[i])){
            ppid[j] = ppid_str[i];
            ++j;
        }
        ++i;
    }

    printf("%s\n", ppid);   //вывод: [ppid]

    fclose(status_file);
    
    return 0;
}