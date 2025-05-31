#include "haed.h"

char* ptr_shmema = NULL;
int   fd_shmema  = -1;

typedef void (*sighandler_t)(int); 


int open_shmema()
{
    /* открытие виртуального объекта разделяемой памяти */   /* если до  этого  момента объект не был создан, произойдет ошибка */
    /* /dev/shm */
    /* возвращает файловый дескриптор  */
    fd_shmema = shm_open("/shmema", O_RDWR, 0666);
    if(fd_shmema == -1)
    {
        perror("shm_open");
        return 1;
    }
   
    ptr_shmema = mmap(NULL, BUFFLEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shmema, 0);
    if(!ptr_shmema)
    {
        perror("mmap");
        close(fd_shmema);
        return 1;
    }

    return 0;
}


void sigusr2_handler(int sig)
{
    time_t currenttime = time(NULL);
    struct tm *loctime = localtime(&currenttime);

    puts("\nGet message:");
    fprintf(stdout, "-%s(%d:%d:%d)\n\n", ptr_shmema, 
                                           loctime -> tm_hour, 
                                           loctime -> tm_min, 
                                           loctime -> tm_sec);
}


int main(void) 
{
    int companion_pid;
    fprintf(stdout, "My pid: %d\n", getpid());
    fprintf(stdout, "Write your companion pid: ");
    scanf("%d", &companion_pid);
    getchar();

    struct sigaction action = 
    {
        .sa_handler = sigusr2_handler
    };

    sigaction(SIGUSR2, &action, NULL);

    if(open_shmema() == 1)
    {
        exit(EXIT_FAILURE);
    }
    
    int histfd = open("./history.txt", O_WRONLY | O_APPEND);
    if(histfd == -1)
    {
        perror("open");
        cleanup_shm(ptr_shmema, BUFFLEN, fd_shmema, "/shmema");
        exit(EXIT_FAILURE);
    }


    pause();  /* ожидаю сигнал от companion1 */
    char msgbuf[BUFFLEN];

    for(;;)
    {
        fputs("Write your message (Type 'exit' to finish correspondence):\n", stdout);
        fgets(msgbuf, BUFFLEN, stdin);

        if(strcmp(msgbuf, "exit\n") == 0)
        {
            break;
        }

        write(histfd, msgbuf, strlen(msgbuf));
        fsync(histfd);

        strcpy(ptr_shmema, msgbuf);    
        
        kill(companion_pid, SIGUSR1);
        pause();
    }

    cleanup_shm(ptr_shmema, BUFFLEN, fd_shmema, "/shmema");
    close(histfd);

    fprintf(stdout, "\nEXIT2\n");
    exit(EXIT_SUCCESS);    
}