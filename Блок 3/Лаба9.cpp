#include <stdio.h>
#include <locale.h>

int main()
{
	setlocale(LC_ALL, "Rus");
	printf("Задание 2\n");
	float time;
	int cost, cost_over;
	printf("Введите время разговоров: ");
	scanf_s("%f", &time);
	printf("Введите стоимость абоненской платы: ");
	scanf_s("%d", &cost);
	printf("Введите стоимость абоненской платы сверх лимита: ");
	scanf_s("%d", &cost_over);
	float s = (cost + ((time - 499) * cost_over));
	if (time > 500) { printf("Итоговая стоимость %f\n\n", s); }
	else { printf("Итоговая стоимость %d\n\n", cost); }

	printf("Задание 5\n");
	int n = 12;
	float bank_interest = 0, bank_deposit;
	printf("Введите количество средств на банковском счету: ");
	scanf_s("%f", &bank_deposit);
	printf("Введите процентную ставку: ");
	scanf_s("%f", &bank_interest);
	float s1 = bank_deposit;
	for (int i = 1; i <= n; i++)
	{
		s1 = s1 + (bank_deposit * ((bank_interest / 100) * i / 12));
		printf("Состояние счёта в конце %d месяца = %f\n", i, s1);
	}
	return 0;
}