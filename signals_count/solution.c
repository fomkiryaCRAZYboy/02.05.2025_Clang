/*
    Разработать приложение, умеющее обрабатывать сигналы SIGUSR1 ,SIGUSR2, SIGTERM.
    После старта Ваше приложение должно по приходу одного из сигналов SIGUSR1, SIGUSR2 
    выполнять суммирование числа срабатываний каждого из сигналов, а после прихода 
    сигнала SIGTERM, требуется вывести в стандартный поток вывода 2 числа, разделенных 
    пробелом, соответствующих количеству обработанных сигналов SIGUSR1, SIGUSR2, и 
    завершить программу. Вывод оканчивается символом конца строки.
*/

#define _POSIX_C_SOURCE 202412L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

typedef void (*sighandler_t)(int); 

volatile sig_atomic_t countusr1 = 0;
volatile sig_atomic_t countusr2 = 0;
volatile sig_atomic_t terminate = 0;



void hndlusr(int sig, siginfo_t* siginfo, void* ucontext)
{
    if(sig == SIGUSR1) ++countusr1;
    else ++countusr2;
}


void hndlterm(int sig, siginfo_t* siginfo, void* ucontext)
{
    terminate = 1;
}


int main(void)
{
    
    int f = open("./pidfile", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(f == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    } 

    char pidbuf[20];
    sprintf(pidbuf, "%d", getpid());

    write(f, pidbuf, strlen(pidbuf));
    close(f);
    
    /* ---------------------------------------------------------------- */


    struct sigaction actusr = 
    {
        .sa_sigaction = hndlusr,
        .sa_flags = SA_SIGINFO
    };

    struct sigaction actterm = 
    {
        .sa_sigaction = hndlterm,
        .sa_flags = SA_SIGINFO
    };

    sigaction(SIGUSR1, &actusr, NULL);
    sigaction(SIGUSR2, &actusr, NULL);
    sigaction(SIGTERM, &actterm, NULL);
    
    /* пока флаг terminate не поменяет значение, ждём и обрабатываем сигналы */
    while(!terminate)
    {
        pause();
    }

    fprintf(stdout, "%d %d", (int)countusr1, (int)countusr2);
    exit(EXIT_SUCCESS);
}