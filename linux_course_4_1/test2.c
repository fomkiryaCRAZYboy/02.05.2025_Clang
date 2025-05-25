#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int fd = open("./test_file.txt", O_RDONLY);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    char* out = malloc(20);
    if(!out)
    {
        perror("malloc");

        close(fd);
        return 1;
    }

    ssize_t readed_bytes = read(fd, out, 11);
    fprintf(stdout, "Was readed: %d bytes\nOut strlen = %d\n\n", readed_bytes, strlen(out));

    fprintf(stdout, "%s\n", out);

    free(out);
    close(fd);
    return 0;
}