#ifndef AREA.H
#define AREA.H
#include <math.h>
#include "struct_ring.h"
float area(struct ring Ring)
{
	float pi = 22 / 7;
	float s = (pi * pow(Ring.R, 2)) - (pi * pow(Ring.r, 2));
	return s;
}
#endif /*AREA.H*/