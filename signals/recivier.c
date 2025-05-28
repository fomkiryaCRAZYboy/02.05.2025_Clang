#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_PID_LEN 20


typedef void (*sighandler_t)(int); 

void sigint_handler(int sig)
{
    puts("\nCan't stop this program");
}

int main(void)
{
    int i = 0;
    pid_t currpid = getpid();
    fprintf(stdout, "pid: %d\n", currpid);


    struct sigaction action = 
    {
        .sa_handler = sigint_handler
    };
    
    /*  signal(SIGINT, sigint_handler);   /* SIGINT = 2 */
    sigaction(SIGINT, &action, NULL);


    int fd = open("./writefile.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);  /* 0644 ~ -rw-r--r-- */
    if(fd == -1)
    {
        perror("openfail");
        exit(EXIT_FAILURE);
    }
    char buf[4];
    
    pid_t sender_stop_pid = fork();
    if(sender_stop_pid == -1)
    {
        perror("forkfail");
        goto fail;
    }

    if(sender_stop_pid > 0)
    {
        for(;;++i)
        {
            sprintf(buf, " %d  ", i);

            if(write(fd, buf, 4) != 4)
            {
                perror("\nwritefail");
                goto fail;
            }
            fsync(fd);
            sleep(1);
        }  
    }  

    sleep(10);

    char fd_str[20];
    snprintf(fd_str, 20, "%d", fd);

    char str_currpid[MAX_PID_LEN];
    snprintf(str_currpid, MAX_PID_LEN, "%d", currpid);

    char* args[] = 
    {
        "./sender_stop",
        str_currpid,
        fd_str,
        NULL
    };

    execvp("./sender_stop", args);
    perror("execfail");
    exit(EXIT_FAILURE);

fail:
    close(fd);
    exit(EXIT_FAILURE);
}