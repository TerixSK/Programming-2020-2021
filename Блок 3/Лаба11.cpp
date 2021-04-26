#include <stdio.h>
#include <locale.h>
#include "struct_ring.h"
#include "perimetr.h"
#include "area.h"

int main()
{
	setlocale(LC_ALL, "Rus");
	ring o;
	printf("������� ���������� ����� x: ");
	scanf_s("%f", &o.x);
	printf("������� ���������� ����� y: ");
	scanf_s("%f", &o.y);
	printf("������� ������ ������� ����������: ");
	scanf_s("%f", &o.r);
	printf("������� ������ ������� ����������: ");
	scanf_s("%f", &o.R);

	float P = perimetr(o.r, o.R);
	printf("�������� = %f\n", P);

	float S = area(o.r, o.R);
	printf("�������  = %f\n", S);
	return 0;
}