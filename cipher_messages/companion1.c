#include "haed.h"
#include "cypher/cipher.h"

char* ptr_shmema = NULL;
int   fd_shmema  = -1;

typedef void (*sighandler_t)(int); 


int create_shmema()
{
    /* создание виртуального объекта разделяемой памяти */
    /* /dev/shm */
    /* возвращает файловый дескриптор  */
    fd_shmema = shm_open("/shmema", O_CREAT | O_RDWR, 0666);
    if(fd_shmema == -1)
    {
        perror("shm_open");
        return 1;
    }
   
    ptr_shmema = display_shm("/shmema", BUFFLEN, fd_shmema);
    if(!ptr_shmema)
    {
        fprintf(stderr, "Failed to display shm\n");
        return 1;
    }

    return 0;
}

/* обработчик сигнала */
void sigusr1_handler(int sig)
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
        .sa_handler = sigusr1_handler
    };

    sigaction(SIGUSR1, &action, NULL);

    if(create_shmema() == 1)
    {
        exit(EXIT_FAILURE);
    }

    int histfd = open("./history.txt", O_WRONLY | O_TRUNC);
    if(histfd == -1)
    {
        perror("open");
        cleanup_shm(ptr_shmema, BUFFLEN, fd_shmema, "/shmema");
        shm_unlink("/shmema");
        exit(EXIT_FAILURE);
    }


    u8 msgbuf[BUFFLEN];
    for(;;)
    {
        fputs("Write your message (Type 'exit' to finish correspondence):\n", stdout);
        fgets(msgbuf, BUFFLEN, stdin);

        if(strcmp(msgbuf, "exit\n") == 0)
        {
            break;
        }

        u8* encrypted_msg = encryption(msgbuf, strlen(msgbuf));
        write(histfd, encrypted_msg, strlen(encrypted_msg));
        fsync(histfd);
    
        strcpy(ptr_shmema, encrypted_msg);
    
        kill(companion_pid, SIGUSR2);
        pause();
    }

    
    cleanup_shm(ptr_shmema, BUFFLEN, fd_shmema, "/shmema");
    shm_unlink("/shmema");
    close(histfd);

    fprintf(stdout, "\nEXIT\n");
    exit(EXIT_SUCCESS);    
}