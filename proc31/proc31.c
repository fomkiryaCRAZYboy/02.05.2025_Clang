/*+Proc31. Описать функцию IsPalindrome(K), возвращающую True, 
если целый параметр K (> 0) является палиндромом (т. е. его запись читается 
одинаково слева направо и справа налево), и False в противном случае. 
С ее помощью найти количество палиндромов в наборе из 10 целых положительных чисел.

Выполнил: Фоминых Кирилл Дмитриевич
03.05.2025
Время выполнения: 12 мин
*/


#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define N   5

bool isPalindrome(int k){
    char num[32];
    sprintf(num, "%d", k);
    
    char reverse_num[32];
    int j = 0;
    for(int i = strlen(num)-1; i >= 0; i--){
        reverse_num[j] = num[i];
        j++;
    }
    reverse_num[j] = '\0';

    if(strcmp(num, reverse_num) == 0) return true;
    return false;
}

int main()
{  
    int arr[N] = {11, 101, 101, 101, 101};

    int count = 0;
    for(int i = 0; i < N; i++){
        if(isPalindrome(arr[i])) count++;
    }

    printf("count = %d", count);

    return 0;
}