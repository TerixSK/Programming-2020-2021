#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <random>
#include <string.h>

// Задание 5
char del(char* str1, int j) // Рекрусивная функция удаления лишнего пробела
{
	int i;
	for (i = j; i < strlen(str1) - 1; i++)
		str1[i] = str1[i + 1];
	str1[i] = 0;
	return *str1;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	printf("Задание 5\n");
	char str[256];
	gets_s(str);
	for (int i = 0; i < 256; i++)
	{
		if (str[i] == ' ' and str[i + 1] == ' ') // если два пробелла подряд
			del(str, i);
	}
	for (int i = strlen(str); i > 0; i--) // если пробелы после последней точки
	{
		if (str[i] == '.')
		{
			int j = i + 1;
			while (str[j] == ' ')
				del(str, j);
			i = 0;
		}
	}
	for (int i = 0; i < strlen(str); i++) // если пробелы после открывающегося знака
	{
		if (str[i] == ' ' and str[i + 1] == '(')
			del(str, i);
		if (str[i] == ' ' and str[i + 1] == '<')
			del(str, i);
		if (str[i] == ' ' and str[i + 1] == '"')
			del(str, i);
	}
	printf("%s", str);
	return 0;
}