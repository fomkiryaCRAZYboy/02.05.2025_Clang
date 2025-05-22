/*
Разработать программу solution, которая по заданному pid, 
осуществляет поиск пути в дереве процессов до процесса с идентификатором 1 (init/systemd). 
Для каждого найденного процесса печатается в отдельной строке его идентификатор.
*/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <ctype.h>

#include <limits.h>

#define MAX_PID_LEN 13

int get_ppid(pid_t curr_pid){
    char path_to_status_file[PATH_MAX];
    snprintf(path_to_status_file, PATH_MAX, "/proc/%d/status", curr_pid);

    FILE* status_file = fopen(path_to_status_file, "r");
    if(!status_file){
        fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
        return -1;
    }

    char ppid_str[256];   //получаю строку: PPid:    [ppid]
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

    fclose(status_file);

    ppid[j] = '\0';
    int p = atoi(ppid);
    return p;
}

int main(int argc, char *argv[])
{
    int pid = -1;

    if (argc > 1) {
        pid = atoi(argv[1]);
    } else {
        printf("err\n");
        return 1;
    }

    printf("%d\n", pid);
    int ppid = -1;
    while(ppid != 1){
        ppid = get_ppid(pid);
        if(ppid == -1){
            printf("failed");
            return 0;
        }
        printf("%d\n", ppid);
        pid = ppid;
    }

    return 0;
}