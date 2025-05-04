/*!File39:
Дан файл целых чисел. Продублировать в нем все числа, принадлежащие диапазону 5–10. 

Выполнил: Фоминых Кирилл Дмитриевич
03.05.2025
Время выполнения: 
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_ARR_SIZE    300

void dublicate_nums_in_arr(int* arr, int* size);

int main(void)
{
    int file_d = open("./file39_test.txt", O_RDONLY);
    if(file_d == -1) {
        perror("./file39_test.txt");
        return 1;
    }

    char buffer[MAX_ARR_SIZE];
    ssize_t readed_bytes = read(file_d, buffer, sizeof(buffer));
    if(readed_bytes <= 0){
        close(file_d);
        fprintf(stderr, "Failed to read data from file\n");
        return 1;
    }

    int numbers_array[MAX_ARR_SIZE];
    int num_idx = 0;
    char curr_num[12]; 

    int buffer_idx = 0;
    int curr_num_idx = 0;
    while(readed_bytes) {
        if(isdigit(buffer[buffer_idx])){
            curr_num[curr_num_idx] = buffer[buffer_idx++];
            curr_num_idx++;
        }

        if((isspace(buffer[buffer_idx]) || buffer[buffer_idx] == '\n') && curr_num_idx){
            //нуль-терминирую массив текущего числа, после того как встречу символ отличный от цифры
            curr_num[curr_num_idx] = '\0';

            numbers_array[num_idx++] = atoi(curr_num);

            curr_num_idx = 0;
        }

        buffer_idx++;
        readed_bytes--;
    }

    dublicate_nums_in_arr(numbers_array, &num_idx);

    close(file_d);

    FILE* file = fopen("file39_test.txt", "w");
    if(!file){
        perror("file39_test.txt");
        return 1;
    }

    for (int i = 0; i < num_idx; i++) {
        fprintf(file, "%d ", numbers_array[i]);
    }

    fclose(file);

    return 0;
}

void dublicate_nums_in_arr(int* arr, int* size) {
    for (int i = 0; i < *size; i++) {
        if (arr[i] >= 5 && arr[i] <= 10) {
            // Сдвигаем элементы вправо
            for (int j = *size; j > i; j--) {
                arr[j] = arr[j-1];
            }
            (*size)++;  
            i++;
        }
    }
}