/* слздание процесса-демона */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(void)
{
    pid_t daemon_pid = fork();
    if(daemon_pid < 0){
        perror("fork");
        return 1;
    }
    if(daemon_pid > 1){
        //при создании демона, завершаю родительский процесс
        _exit(0); 
    }

    //действия в дочернем процессе демоне

    //меняем рабочую директорию процесса на корневую (чтобы не было проблем с монтированием файловой системы во время работы демона)
    if(chdir("/") != 0){
        perror("chdir");
        return 0;
    } 

    //устанавливаю новую сессию для демона и делаю его лидером новой группы, чтобы он не был привязан к родительскому процессу
    pid_t new_session_id = setsid();
    if(new_session_id == -1){ // неудача создания сессии
        fprintf(stderr, "errno %d: %s", errno, strerror(errno));
        _exit(1);
        return 1;
    }

    //pid демона
    //можно получить pid процесса-демона, посредством вывода в stdout, но это плохая практика, т.к. демоны не должны взаимодействовать со стандартными потоками
    //fprintf(stdout, "%d", getpid());

    //если нужно получит pid демона, лучше записать его в файл

    //демон не должен взаимодействовать со стандартными потоками i/o, поэтому закрываю их
    close(STDERR_FILENO);
    close(STDOUT_FILENO);
    close(STDIN_FILENO);

    sleep(1000); // демон будет работать на фоне   
    return 0;
}