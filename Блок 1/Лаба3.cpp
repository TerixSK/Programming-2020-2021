#include <stdio.h>
#include <math.h>

int main(void)
{
	// ������� 1
	int n;
	scanf_s("%d", &n);
	printf("\n");

	// ������� 2 (������� � 16-���� �������)
	printf("Task 2\n");
	printf("%x\n", n);
	printf("\n");

	// ������� 3 (������� � 16-���� ������� � ����� ����� �� 4)
	printf("Task 3\n");
	printf("%x\n", n);
	printf("\n");
	printf("%x\n", n << 4);
	printf("\n");

	// ������� 4 (�������� �����)
	printf("Task 4\n");
	printf("%x\n", n);
	printf("\n");
	printf("%x\n", -n);
	printf("\n");

	// ������� 5
	int n1;
	printf("Task 5\n");
	scanf_s("%x\n", &n1);
	printf("\n");
	printf("%x", n & n1);
	return 0;
}