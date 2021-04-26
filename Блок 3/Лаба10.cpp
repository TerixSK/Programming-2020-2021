#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <random>
#include <string.h>

// Задание 2
struct point // Структура для описания точек на плоскости
{
	int x, y;
};
int distance(const point a, const point b) // Расчёт дистанции для двух точек
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

// Задание 5
void del(char str1[], char charToDel)
{

	int index = 0;

	while (str1[index] != '\0')
	{
		if (str1[index] == charToDel) {
			str1[index] = str1[index + 1];
		}

		index++;
	}

	printf("%s", str1);
}
int main()
{
	setlocale(LC_ALL, "Rus");
	printf("Задание 2\n");
	int n;
	point p[100];
	int arr[100][100];
	printf("Введите размер плоскости: ");
	scanf_s("%d", &n);
	for (int i = 0; i < n; i++) // задаём элементы в диапозоне
	{
		p[i].x = rand() % 100;
		p[i].y = rand() % 100;
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			arr[i][j] = distance(p[i], p[j]); // использование функции
	for (int i = 0; i < n; i++) // вывод массива
	{
		for (int j = 0; j < n; j++)
			if (i != j)
				printf("%d ", arr[i][j]);
		printf("\n");
	}
	printf("\n");

	printf("Задание 5\n");
	char str[256] = "Jopa  est trusi";
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] == ' ' and str[i + 1] == ' ')
			del(str, ' ');
	}
	return 0;
}
