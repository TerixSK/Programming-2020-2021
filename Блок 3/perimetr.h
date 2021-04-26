#ifndef PERIMETR.H
#define PERIMETR.H
#include "struct_ring.h"

float perimetr(struct ring Ring)
{
	float pi = 22 / 7;
	float perim = (2 * pi * Ring.R) - (2 * pi * Ring.r);
	return perim;
}
#endif /*PERIMETR.H*/