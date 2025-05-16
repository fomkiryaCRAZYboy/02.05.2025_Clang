/* Подсчет числа процессов с заданным именем */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

#define MAX_CMD_LEN 50

void write_process_stat(char pid[NAME_MAX]){
    char command[MAX_CMD_LEN];
    snprintf(command, MAX_CMD_LEN, "cat /proc/%s/stat > proc_stat.txt", pid);
    system(command);
}

char* check_prosess_name(){
    FILE* proc_stat = fopen("proc_stat.txt", "r");
    if(!proc_stat){
        fprintf(stderr, "errno %d: %s", errno, strerror(errno));
        return NULL;
    }

    char* process_name = malloc(NAME_MAX);
    if(!process_name){
        fprintf(stderr, "Faeiled to allocate memory\n");
        return NULL;
    }   

    char buff[300];
    fgets(buff, 300, proc_stat);
    fclose(proc_stat);

    int j = 0;
    for(int i = 0; i < 300; i++){
        if(buff[i] == '('){
            ++i;
            while(buff[i] != ')'){
                process_name[j] = buff[i++];
                ++j;
            }
        }
    }

    process_name[j] = '\0';
    return process_name;
}

int main(void)
{
    char proc_name[NAME_MAX] = "genenv"; // необходимо найти количество процессов с таким именем
    int count = 0;

    DIR* proc_dir = opendir("/proc");
    if(!proc_dir){
        fprintf(stderr, "errno %d: %s", errno, strerror(errno));
        return 0;
    }

    struct dirent* entry;
    while(entry = readdir(proc_dir)){
        char entry_name[NAME_MAX];
        strcpy(entry_name, entry->d_name);

        //если имя вхождения начинается с числа, то это каталог процесса (имя - это его pid)
        if(isdigit(entry_name[0])){
            write_process_stat(entry_name);
            char* name = check_prosess_name();

            if(!name){
                closedir(proc_dir);
                return 1;
            }

            if(strcmp(name, proc_name) == 0) ++count;
            free(name);
        }
    }

    printf("%d\n", count);
    closedir(proc_dir);

    return 0;
}