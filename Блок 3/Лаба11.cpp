#include <stdio.h>
#include <locale.h>
#include "struct_ring.h"
#include "perimetr.h"
#include "area.h"

int main()
{
	setlocale(LC_ALL, "Rus");
	ring o;
	printf("¬ведите координату цента x: ");
	scanf_s("%f", &o.x);
	printf("¬ведите координату цента y: ");
	scanf_s("%f", &o.y);
	printf("¬ведите радиус меньшей окружности: ");
	scanf_s("%f", &o.r);
	printf("¬ведите радиус большей окружности: ");
	scanf_s("%f", &o.R);

	float P = perimetr(o.r, o.R);
	printf("ѕериметр = %f\n", P);

	float S = area(o.r, o.R);
	printf("ѕлощадь  = %f\n", S);
	return 0;
}