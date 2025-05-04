/*
Array 136:
Дано множество A из N точек (N > 2, точки заданы своими координатами x, y). 
Найти такую точку из данного множества, сумма расстояний от которой 
до остальных его точек минимальна, и саму эту сумму.

Выполнил: Фоминых Кирилл Дмитриевич
02.05.2025
Время выполнения: 
*/

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>

// Структура данных точки в двумерном пространстве
typedef struct {
    double x;  
    double y;
} POINT;

// Функция вычисления расстояния между двумя точками
double calculate_distance(const POINT* point1, const POINT* point2) {
    double dx = point1->x - point2->x;
    double dy = point1->y - point2->y;
    return sqrt(dx*dx + dy*dy);  
}

// Проверка на совпадение точек
bool are_points_equal(const POINT* a, const POINT* b) {
    return a->x == b->x && a->y == b->y;
}

int main(void) 
{
    POINT points_array[] = {
        {10.0, 5.0},
        {0.0, 0.0},
        {100.0, 14.0},
        {-15, 0},
    };
    
    const int N = sizeof(points_array) / sizeof(points_array[0]);
    
    if (N <= 2) {
        printf("Error: Number of points must be greater than 2.\n");
        return 1;
    }

    double min_sum_distance = DBL_MAX;  // Используем максимальное значение double
    int point_index = -1;
    
    for(int i = 0; i < N; i++) {
        double cur_sum_distance = 0.0;
        bool has_duplicate = false;
        
        // Проверка на дубликаты
        for (int j = 0; j < N; j++) {
            if (i != j && are_points_equal(&points_array[i], &points_array[j])) {
                has_duplicate = true;
                break;
            }
        }
        if (has_duplicate) continue;
        
        for(int j = 0; j < N; j++) {
            if(j == i) continue;
            cur_sum_distance += calculate_distance(&points_array[i], &points_array[j]);
        }

        if(cur_sum_distance < min_sum_distance) {
            point_index = i;
            min_sum_distance = cur_sum_distance;
        }
    }

    if (point_index == -1) {
        printf("All points are identical.\n");
    } else {
        printf("Minimal sum of distances = %.2f\n", min_sum_distance);
        printf("Point coordinates: x = %.2f, y = %.2f\n", 
               points_array[point_index].x, points_array[point_index].y);
    }

    return 0;
}