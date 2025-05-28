#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



int main(int argc, char* argv[])
{   
    if(argc != 3)
    {
        fprintf(stderr, "incorrect args\n");
        exit(EXIT_FAILURE);
    }

    pid_t target_pid = atoi(argv[1]);
    if(kill(target_pid, SIGSTOP) != 0)
    {
        perror("killfail");
        goto fail;
    }

    fprintf(stdout, "Process stopped\n");

    FILE* cont_fd = fopen("cont_proc.txt", "r");
    if(!cont_fd)
    {
        perror("fopenfail");
        goto fail;
    }

    char buff[4] = {'\0'};
    if(!fgets(buff, 3, cont_fd))
    {
        perror("fgetsfail");
        fclose(cont_fd);
        goto fail;
    }


    int cont = 0;
    if(strcmp(buff, "yes") == 0)
    {
        cont = 1;
    }

    for(;;){
        sleep(5);
        fprintf(stdout, "Type 'y' to continue process\n");

        if(!cont)
        {
            printf("get yes\n");
            if(kill(target_pid, SIGCONT) != 0)
            {
                perror("killfail");
                fclose(cont_fd);
                goto fail;
            }

            fprintf(stdout, "Procces continued\n");
            fclose(cont_fd);
            close(atoi(argv[2]));
            exit(EXIT_SUCCESS);
        }
    }

fail:
    close(atoi(argv[2]));
    exit(EXIT_FAILURE);
}