#include <stdio.h>
#include <locale.h>

int main()
{
	setlocale(LC_ALL, "Rus");
	printf("������� 2\n");
	float time;
	int cost, cost_over;
	printf("������� ����� ����������: ");
	scanf_s("%f", &time);
	printf("������� ��������� ���������� �����: ");
	scanf_s("%d", &cost);
	printf("������� ��������� ���������� ����� ����� ������: ");
	scanf_s("%d", &cost_over);
	float s = (cost + ((time - 499) * cost_over));
	if (time > 500) { printf("�������� ��������� %f\n\n", s); }
	else { printf("�������� ��������� %d\n\n", cost); }

	printf("������� 5\n");
	int n = 12;
	float bank_interest = 0, bank_deposit;
	printf("������� ���������� ������� �� ���������� �����: ");
	scanf_s("%f", &bank_deposit);
	printf("������� ���������� ������: ");
	scanf_s("%f", &bank_interest);
	float s1 = bank_deposit;
	for (int i = 1; i <= n; i++)
	{
		s1 = s1 + (bank_deposit * ((bank_interest / 100) * i / 12));
		printf("��������� ����� � ����� %d ������ = %f\n", i, s1);
	}
	return 0;
}