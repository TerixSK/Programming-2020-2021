#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <random>
#include <string.h>

// ������� 2
struct point // ��������� ��� �������� ����� �� ���������
{
	int x, y;
};
int distance(const point a, const point b) // ������ ��������� ��� ���� �����
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

int main()
{
	setlocale(LC_ALL, "Rus");
	printf("������� 2\n");
	int n;
	point p[100];
	int arr[100][100];
	printf("������� ������ ���������: ");
	scanf_s("%d", &n);
	for (int i = 0; i < n; i++) // ����� �������� � ���������
	{
		p[i].x = rand() % 100;
		p[i].y = rand() % 100;
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			arr[i][j] = distance(p[i], p[j]); // ������������� �������
	for (int i = 0; i < n; i++) // ����� �������
	{
		for (int j = 0; j < n; j++)
			if (i != j)
				printf("%d ", arr[i][j]);
		printf("\n");
	}
	printf("\n");
	return 0;
}