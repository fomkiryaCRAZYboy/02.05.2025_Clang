#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#define CMD     100

int main(void)
{
    int finfo = -1;
    char cmd[CMD];
    char zone[6];

    finfo = open("./finfo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(finfo == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* 8 ядер - 8 итераций */
    for(int i = 0; i < 7; ++i)
    {
        sprintf(zone, "ZONE %d: ", i);
        write(finfo, zone, 6);

        snprintf(cmd, CMD, "cat /sys/class/thermal/thermal_zone%d/temp > finfo.txt", i);
        system(cmd);
    }

    exit(EXIT_FAILURE);
}