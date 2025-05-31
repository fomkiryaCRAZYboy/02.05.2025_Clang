/* создание процесса-демона */
/* 
    Задача -- снабдить демон обработчиком сигнала SIGURG, 
    по приходу которого демон должен завершать свою работу.
*/

#define _POSIX_C_SOURCE 202412L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>


//int fd = -1;

/* функция-обработчик сигнала */
void sigurgaction(int signum, siginfo_t* sinfo, void* ucontext)
{
    /*
    write(fd, "EXIT", 5);
    close(fd);
    */
    exit(EXIT_SUCCESS);
}


int main(void)
{
    pid_t daemon_pid = fork();
    if(daemon_pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(daemon_pid > 1)
    {
        //при создании демона, завершаю родительский процесс
        _exit(0); 
    }


    //действия в дочернем процессе демоне

    /* обработчик для сигнала SIIGURG */
    struct sigaction action = 
    {
        .sa_sigaction = sigurgaction,
        .sa_flags = SA_SIGINFO
    };

    sigaction(SIGURG, &action, NULL);


    /*
    fd = open("./pid.txt", O_WRONLY | O_TRUNC);
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    */

    /*
    char buff[20];
    snprintf(buff, 20, "%d", getpid());
    write(fd, buff, strlen(buff));
    */
    
    //меняем рабочую директорию процесса на корневую (чтобы не было проблем с монтированием файловой системы во время работы демона)
    if(chdir("/") != 0)
    {
        perror("chdir");
        exit(EXIT_FAILURE);
    } 

    //устанавливаю новую сессию для демона и делаю его лидером новой группы, чтобы он не был привязан к родительскому процессу
    pid_t new_session_id = setsid();
    if(new_session_id == -1)
    {    // неудача создания сессии
        fprintf(stderr, "errno %d: %s", errno, strerror(errno));
        _exit(1);
        exit(EXIT_FAILURE);
    }

    //pid демона
    //можно получить pid процесса-демона, посредством вывода в stdout, но это плохая практика, т.к. демоны не должны взаимодействовать со стандартными потоками
    fprintf(stdout, "%d", getpid());  /* так нельзя */

    //если нужно получит pid демона, лучше записать его в файл

    //демон не должен взаимодействовать со стандартными потоками i/o, поэтому закрываю их
    close(STDERR_FILENO);
    fclose(stdout);    /* close! */
    close(STDIN_FILENO);

    sleep(1000); // демон будет работать на фоне   

    /* close(fd); */
    exit(EXIT_SUCCESS);
}