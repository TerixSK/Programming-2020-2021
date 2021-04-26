#include <stdio.h>
#include <malloc.h>

int main()
{
    // Задание 1
    printf("Task 1\n");
    int arr[4], * p;
    p = arr;
    arr[0] = 0;
    arr[1] = 3;
    arr[2] = 5;
    arr[3] = 7;
    printf("%d %d %d %d", *p, *(p + 1), *(p + 2), *(p + 3));
    printf("\n");
    printf("\n");

    // Задание 2
    printf("Task 2\n");
    int* a;
    a = (int*)malloc(4 * sizeof(int));
    for (int i = 0; i < 4; i++)
    {
        scanf_s("%d", &a[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        printf("%d ", a[i]);
    }
    free(a);
    return 0;
}