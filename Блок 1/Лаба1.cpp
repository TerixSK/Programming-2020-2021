#include <stdio.h>

int main(void)
{
	int a;
	char c;

	scanf_s("%c", &c); // ���� � ����� �������
	printf("%c\n", c);

	scanf_s("%d", &a); // ���� � ����� ������ �����
	printf("%d\n", a);
	return 0;
}