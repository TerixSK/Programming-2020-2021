#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <direct.h>

int main()
{
	setlocale(LC_ALL, "Rus");
	int n; // Кол-во файлов
	printf("Введите количество файлов, которые  вы хотите создать: ");
	scanf_s("%d", &n);
	char name[64];
	char add[5] = { '.', 't', 'x', 't' }; // Вспомогательная строка для добовляние разрешения файлу
	FILE* mainfile = fopen("mainfile.txt", "w+"); // Создание файла для записи имён
	for (int i = 1; i <= n; i++)
	{
		printf("Введите имя файла, который вы хотите создать: ");
		scanf("%s", &name); // Ввод названия файлов
		strncat(name, add, 4); // Добовления разрешения, путём сложения строк
		FILE* file = fopen(name, "w+"); // Создание нового файла
		fclose(file);
		fprintf(mainfile, "%s\n", name); // Вывод в main файл имён
	}
	fclose(mainfile);
	return 0;
}