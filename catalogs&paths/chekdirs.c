//программа проходится по каталогу и записывает в файл catalogs.txt всей файлы/каталоги и тд внутри этого каталога

#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <limits.h> // для PATH_MAX

int main(void)
{
    DIR* dir = opendir(".."); //открываю родительский каталог для каталога catalogs&paths (то есть 02_05_CLANG)
    if(!dir){
        perror("failed to open dir\n");
        return 1;
    }

    FILE* file = fopen("catalogs.txt", "w");
    if(!file) {
        closedir(dir);

        fprintf(stderr, "errno: %d\n", errno);
        perror("catalogs.txt\n");
    }
    
    struct dirent* entry;
    struct stat entry_stat; 

    int i = 1;
    while((entry = readdir(dir)) != NULL) {
        fprintf(file, "%d ENTRTY:\n", i);

        char path_to_entry[PATH_MAX];

        sprintf(path_to_entry, "../%s", entry -> d_name);

        fprintf(file, "Path to entry: %s\n", path_to_entry);
        fprintf(file, "Name entry: %s\n", entry -> d_name);
        
        if (stat(path_to_entry, &entry_stat) == -1) {
            perror("stat");

            fclose(file);
            closedir(dir);
            return 0;
        }

        if(S_ISREG(entry_stat.st_mode)){
            fprintf(file, "Type entry: regular file");
        } else if(S_ISDIR(entry_stat.st_mode)){
            fprintf(file, "Type entry: directory");
        } else{
            fprintf(file, "Another entry type");
        }

        fprintf(file, "\n-----------------\n\n");
        ++i;
    }

    fclose(file);
    closedir(dir);

    return 0;
}