#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <direct.h>

int main()
{
	setlocale(LC_ALL, "Rus");
	int n; // ���-�� ������
	printf("������� ���������� ������, �������  �� ������ �������: ");
	scanf_s("%d", &n);
	char name[64];
	char add[5] = { '.', 't', 'x', 't' }; // ��������������� ������ ��� ���������� ���������� �����
	FILE* mainfile = fopen("mainfile.txt", "w+"); // �������� ����� ��� ������ ���
	for (int i = 1; i <= n; i++)
	{
		printf("������� ��� �����, ������� �� ������ �������: ");
		scanf("%s", &name); // ���� �������� ������
		strncat(name, add, 4); // ���������� ����������, ���� �������� �����
		FILE* file = fopen(name, "w+"); // �������� ������ �����
		fclose(file);
		fprintf(mainfile, "%s\n", name); // ����� � main ���� ���
	}
	fclose(mainfile);
	return 0;
}