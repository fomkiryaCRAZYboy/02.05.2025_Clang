//программа проходится по каталогу и записывает в файл catalogs.txt всей файлы/каталоги и тд внутри этого каталога

#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <limits.h> // для PATH_MAX

#define MAX_OPENED_DIRS 100

DIR* opened_dirs[MAX_OPENED_DIRS];
int opened_dirs_count = 0;

void close_dirs();

int analyse_dir(DIR* dir, FILE* file, const char* current_dir_path);


int main(void)
{
    DIR* dir = opendir(".."); //открываю родительский каталог для каталога catalogs&paths (то есть 02_05_CLANG)
    if(!dir){
        perror("failed to open direct\n");
        return 1;
    }

    opened_dirs[opened_dirs_count++] = dir;

    FILE* file = fopen("catalogs.txt", "w"); //открываю новый файл на запись
    if(!file) {
        close_dirs();

        fprintf(stderr, "errno: %d\n", errno);
        perror("catalogs.txt\n");
    }

    if(analyse_dir(dir, file, "..")) {
        fprintf(stderr, "Failed to analyse");
        close_dirs();
        return 1;
    }

    close_dirs();
    fclose(file);
    
    return 0;
}

int analyse_dir(DIR* dir, FILE* file, const char* current_dir_path) {
    struct dirent* entry;
    struct stat entry_stat;

    int i = 1;
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем . и ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        fprintf(file, "%d ENTRY:\n", i);

        char path_to_entry[PATH_MAX];
        snprintf(path_to_entry, PATH_MAX, "%s/%s", current_dir_path, entry->d_name);

        fprintf(file, "Path to entry: %s\n", path_to_entry);
        fprintf(file, "Name entry: %s\n", entry->d_name);

        if (stat(path_to_entry, &entry_stat) == -1) {
            perror("stat");
            return 1;
        }

        if (S_ISREG(entry_stat.st_mode)) {
            fprintf(file, "Type entry: regular file\n");
        } else if (S_ISDIR(entry_stat.st_mode)) {
            fprintf(file, "Type entry: directory\n");

            if (opened_dirs_count >= MAX_OPENED_DIRS) {
                fprintf(stderr, "Too many opened directories!\n");
                return 1;
            }

            DIR* subdir = opendir(path_to_entry);
            if (!subdir) {
                perror("opendir");
                return 1;
            }

            opened_dirs[opened_dirs_count++] = subdir;
            analyse_dir(subdir, file, path_to_entry);  // Рекурсивный вызов с новым путём

        } else if (S_ISLNK(entry_stat.st_mode)) {
            fprintf(file, "Type entry: symbolic link\n");
        } else {
            fprintf(file, "Another entry type\n");
        }

        fprintf(file, "-----------------\n\n");
        i++;
    }

    return 0;
}

void close_dirs(){
    while(opened_dirs_count){
        closedir(opened_dirs[opened_dirs_count-1]);
        --opened_dirs_count;
    }
}