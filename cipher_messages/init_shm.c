#include "haed.h"

char* display_shm(const char* name_obj, off_t size_obj, int fd)
{
    
    /* установка размера объекта */
    if(ftruncate(fd, size_obj) == -1)
    {
        perror("ftruncate");
        close(fd);
        shm_unlink(name_obj);
        return NULL;
    }
    

    /* отображение объекта в память процесса */
    /* 
        Функция mmap отражает length байтов (1024), начиная со смещения 
        offset файла (0) (или другого объекта), определенного файловым 
        описателем fd, в память, начиная с адреса addr (NULL, так как ядро 
        самостоятельно выберет адрес отображения)
    */

    char *ptr = mmap(NULL, size_obj, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(!ptr)
    {
        perror("mmap");
        close(fd);
        shm_unlink(name_obj);
        return NULL;
    }

    return ptr;
}

void cleanup_shm(char* ptr, off_t size_obj, int fd, const char* name_obj)
{
    /* отключение отображения */
    munmap(ptr, size_obj);
    close(fd);
}