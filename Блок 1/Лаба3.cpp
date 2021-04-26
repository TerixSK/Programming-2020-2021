#include <stdio.h>
#include <math.h>

int main(void)
{
	// Задание 1
	int n;
	scanf_s("%d", &n);
	printf("\n");

	// Задание 2 (вывести в 16-чной системе)
	printf("Task 2\n");
	printf("%x\n", n);
	printf("\n");

	// Задание 3 (вывести в 16-чной системе и сдвиг влево на 4)
	printf("Task 3\n");
	printf("%x\n", n);
	printf("\n");
	printf("%x\n", n << 4);
	printf("\n");

	// Задание 4 (инверсия числа)
	printf("Task 4\n");
	printf("%x\n", n);
	printf("\n");
	printf("%x\n", -n);
	printf("\n");

	// Задание 5
	int n1;
	printf("Task 5\n");
	scanf_s("%x\n", &n1);
	printf("\n");
	printf("%x", n & n1);
	return 0;
}