
/*
Программы test и test2 созданы с целью проверки утверждения:
«Данные записанные в файл с помощью write сразу могут быть прочитаны другим приложением с помощью read».
*/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#include <assert.h>


int main(void)
{

    int test_fd = open("./test_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(test_fd < 0)
    {
        fprintf(stderr, "errno %d: %s", errno, strerror(errno));
        return 1;
    }

    char test_string[] = "test_string";
    int len = strlen(test_string);


    if(write(test_fd, test_string, len) != len)
    {
        perror("write");

        goto clean;
    }

    if(fsync(test_fd) == -1)
    {
        perror("fsync");
        goto clean;
    }
    close(test_fd);


    pid_t child = fork();
    if(child < 0)
    {
        perror("fork");
        goto clean;
    }

    if(child > 0)
    {
        //завершаю родительский процесс
        _exit(0);
    }

    //управление передаётся программе test2
    execlp("./test2", NULL);

    //в случае ошибки errno будет иметь значение -1
    //в случае успеха exec код приведённый ниже исполнен не будет
    perror("exec failed");
    return 0;

clean:
    close(test_fd);
    return 1;
}