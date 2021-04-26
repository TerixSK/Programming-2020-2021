#include <stdio.h>

int main()
{
    // Задание 1
    printf("Task 1\n");
    int arr[] = { 0, 3, 5, 7, 11, 13 }; // Задаём массив
    for (int i = 0; i < 6; i++) // Выводим массив
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("\n");

    // Задание 2
    printf("Task 2\n");
    int a[2][2] = { 1, 1, 1, 4 }; // Задаём два двумерных массивов
    int b[2][2] = { 1, 2, 4, 1 };
    int c[2][2];
    for (int i = 0; i < 2; i++) // Умножение матриц
    {
        for (int j = 0; j < 2; j++)
        {
            c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j];
        }
    }
    printf(" %d  %d\n%d  %d", c[0][0], c[0][1], c[1][0], c[1][1]); // Вывод двумерного массива
    return 0;
}