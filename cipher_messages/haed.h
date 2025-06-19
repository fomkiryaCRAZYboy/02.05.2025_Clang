#define _POSIX_C_SOURCE 202412L

#define BUFFLEN         1024

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

typedef unsigned char  u8;

char* display_shm(const char* name_obj, off_t size_obj, int fd);

void cleanup_shm(char* ptr, off_t size_obj, int fd, const char* name_obj);