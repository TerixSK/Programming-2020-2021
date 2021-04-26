#include <stdio.h>
#include <math.h>

int main(void)
{
	double z1, z2;
	double n, m;

	scanf_s("%lf\n", &n); // Ввод двух чисел m и n
	scanf_s("%lf", &m);

	z1 = (((m - 1) * sqrt(m)) - ((n - 1) * sqrt(n))) / (sqrt(n * pow(m, 3)) + n * m + pow(m, 2) - m);
	z2 = (sqrt(m) - sqrt(n)) / (m);
	printf("%lf\n", z1);
	printf("%lf \n", z2); // Вывод чисел (они длжны быть равны)
	return 0;
}