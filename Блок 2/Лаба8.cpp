#include <stdio.h>
#include <string.h>
#include <locale.h>
int main()
{
    setlocale(LC_ALL, "Ru");
    char str1[256];
    char str2[256];
    gets_s(str1);   
    gets_s(str2);
    printf("\n");
    printf("Первая строка: %s\n", str1);
    printf("Вторая строка: %s\n\n", str2);
    //Задание 3
    printf("Задание 3\n");
    int x = 0;
    // Основной цикл проверки какая строка больше или они равны
    for (int i = 0, j = 0; str1[i] != '\0' && str2[j] != '\0' && x == 0; i++, j++)
    {
        if (str1[i] > str2[j])
        {
            x++;
        }
        else if (str1[i] < str2[j])
        {
            x--;
        }
    }
    if (x > 0) { printf("Первая строка больше второй\n\n"); }
    if (x < 0) { printf("Вторая строка больше первой\n\n"); }
    if (x == 0) { printf("Строки равны\n\n"); }
    //Задание 5
    printf("Задание 5\n");
    char str[256];
    strcpy_s(str, str1);
    strcpy_s(str1, str2); // Копируем данные из первой строки во вторую
    printf("str1 = %s\n\n", str1);
    strcpy_s(str1, str); // Копирование из резервной строки в первую для возвращения изначального значения строки

    //Задание 6
    printf("Задание 6\n");
    int n = 6;
    int k = 0; /// Счётчик
    while (str1[k] != '\0') { k++; } /// Прохождение строки до символа конца строки '\0'
    for (int i = 0; i < n; i++)
    {
        str1[k++] = str2[i]; /// Прибавление в конец 1-й строки первые 6 символов 2-й строки
    }
    str1[k] = '\0';
    printf("str1 = %s\n\n", str1);
    //Задание 10
    printf("Задание 10\n");
    // Поиск одинаковых символов в строках
    int f = 0;
    for (int i = 0; str1[i] != '\0' && !f; i++)
    {
        for (int j = 0; str2[j] != '\0' && !f; j++)
        {
            if (str1[i] == str2[j])
            {
                printf("Character: %c\n\n", str1[i]);
                f = 1;
            }
        }
    }

    //Задание 13
    // Выделить из одной строки кусочки, разделенные любым из множества символов (разделителей), входящих во вторую строку.
    printf("Задание 13\n");
    printf("%s", strtok(str1, str2));
    return 0;
}