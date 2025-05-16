#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

//в момент вызова fork родительский процесс запускаа программы (fork1) "расслаивается" 
//программа исполнится дважды начиная с момента вызова fork, сначала за родительский процесс, потом за дочерний, или наоборот
//следовательно, необходимо проверять, что вернула функция fork()
int main(void)
{
    fprintf(stdout, "Creating new process...\n");

    pid_t new_pid = fork();
    if(new_pid == -1){
        perror("failed to create process\n");
        return 1;
    } 

    else if(new_pid == 0) {   //в дочернем процессе fork() вернёт 0
        printf("CHILD:\n");
        printf("pid = %d, ppid = %d\n", getpid(), getppid());

        if(getchar() == '1'){
            sleep(1);
            _exit(0);
        }

        sleep(3);
        _exit(10);  //завершаю выполнение дочернего процесса с кодом 10
    } 

    else{    //в родительском процессе fork() вернёт pid дочернего процесса

        int status;
        pid_t child_exit_code = wait(&status);
        if(child_exit_code == -1){
            //если у процесса нет детей
            perror("wait failed\n");
            return 1;
        }

        printf("PARENT:\n");
        printf("pid = %d, ppid = %d\n", getpid(), getppid());

        //макрос WIFEXITED, определённый в wait.h, возвращает 0, если дочерний процесс завершёен
        //WEXITSTATUS "расшифровывает" код завершения дочернего процесса
        if (WIFEXITED(status)) {
            if(!WEXITSTATUS(status)){
                printf("child_exit_code(FAILED) = %d", WEXITSTATUS(status));
            } else printf("child_exit_code = %d", WEXITSTATUS(status));

        }
    }

    return 0;
}