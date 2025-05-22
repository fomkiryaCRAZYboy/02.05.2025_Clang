/*
Разработать программу solution, которая по заданному pid, 
осуществляет поиск всех потомков процесса с этим идентификатором 
и выводит их количество (включая процесс с заданным pid).
*/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_PID_LEN 13

//находим количество прямых потомков процесса
//ищем процессы, ppid которых равен curr_pid

int child_count(char curr_pid[MAX_PID_LEN], DIR* proc_dir, int* res_procs_count){
    struct dirent* entry;

    while(entry = readdir(proc_dir)){
        if(isdigit(entry -> d_name[0])){
            char path_to_entry_status[PATH_MAX];
            snprintf(path_to_entry_status, PATH_MAX, "/proc/%s/status", entry -> d_name);

            printf("path: %s\n", path_to_entry_status);

            //открываю файл status у процесса
            FILE* entry_status_f = fopen(path_to_entry_status, "r");
            if(!entry_status_f){
                fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
                return -1;
            }

            //запрашиваю память в куче, так как стек сильно "засорится" после множества рекурсивных вызовов
            char* entry_ppid_str = malloc(MAX_PID_LEN+10);
            if(!entry_ppid_str){
                fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));

                fclose(entry_status_f);
                return -1;
            }

            //записываю строку "PPid:   [PPID]"
            while(fgets(entry_ppid_str, MAX_PID_LEN+10, entry_status_f)){
                if(strstr(entry_ppid_str, "PPid:")) break;
            }
            
            char* entry_ppid_num = malloc(MAX_PID_LEN);
            if(!entry_ppid_num){
                fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));

                fclose(entry_status_f);
                free(entry_ppid_str);
                
                return -1;
            }
            printf("entry_ppid_str = %s\n", entry_ppid_str);

            int idx = 0;
            int jdx = 0;

            //записываю [PPID]
            while(entry_ppid_str[idx]){
                if(isdigit(entry_ppid_str[idx])){
                    entry_ppid_num[jdx] = entry_ppid_str[idx];
                    ++jdx;
                }
                ++idx;
            }

            entry_ppid_num[jdx] = '\0';
            printf("entry_ppid_num = %s\n", entry_ppid_num);

            //если [PPID] не равен curr_pid, перехожу к следующему процессу, т.к. данный процесс - не потомок
            if(strcmp(entry_ppid_num, curr_pid) != 0){
                fclose(entry_status_f);
                free(entry_ppid_str);
                free(entry_ppid_num);

                continue;
            } 
            //увеличиваю res_procs_count и рекурсивно вызываю функцию подсчёта потомков для этого процесса 
            else{
                (*res_procs_count)++;
                printf("----PLUS----\n");

                //освобождаю память от строк с ppid, так как они больше не нужны
                free(entry_ppid_str);
                free(entry_ppid_num);

                //получаю pid данного процесса
                char* entry_pid_str = malloc(MAX_PID_LEN+10);
                if(!entry_pid_str){
                    fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
                    fclose(entry_status_f);

                    return -1;
                }

                //сбрасываем файловую позицию для чтения с начала файла
                fseek(entry_status_f, 0, SEEK_SET);

                //записываю строку "Pid:   [PID]"
                while(fgets(entry_pid_str, MAX_PID_LEN+10, entry_status_f)){
                    if(strstr(entry_pid_str, "Pid:")) break;
                }

                printf("ENTRYPIDSTR = %s\n", entry_pid_str);

                char entry_pid_num[MAX_PID_LEN];
                int j = 0;
                int i = 0;

                //записываю [PID]
                while(entry_pid_str[i]){
                    if(isdigit(entry_pid_str[i])){
                        entry_pid_num[j] = entry_pid_str[i];
                        ++j;
                    }
                    ++i;
                }

                entry_pid_num[j] = '\0';

                free(entry_pid_str);
                fclose(entry_status_f);

                printf("entryPid_num = %s\n", entry_pid_num);
                child_count(entry_pid_num, proc_dir, res_procs_count);
            }
        }
    }

    return 0;

}

int main(int argc, char* argv[])
{
    char curr_pid[MAX_PID_LEN];

    if (argc > 1) {
        strcpy(curr_pid, argv[1]);
    } else {
        printf("err\n");
        return 1;
    }

    DIR* proc_dir = opendir("/proc");
    if(!proc_dir){
        fprintf(stderr, "errno %d: %s", errno, strerror(errno));
        return 1;
    }

    int child_procs_count = 1; //учитываем переданный в main процесс
    if(child_count(curr_pid, proc_dir, &child_procs_count) != 0) {
        printf("fail\n");
        closedir(proc_dir);
        return 1;
    }

    printf("child_procs_count = %d\n", child_procs_count);

    closedir(proc_dir);
    return 0;
}