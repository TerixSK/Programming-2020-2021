#include <stdio.h>

int main(void)
{
	int a;
	char c;

	scanf_s("%c", &c); // ¬вод и вывод символа
	printf("%c\n", c);

	scanf_s("%d", &a); // ¬вод и вывод целого числа
	printf("%d\n", a);
	return 0;
}