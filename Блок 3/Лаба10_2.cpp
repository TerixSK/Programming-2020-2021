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
	return 0;
}