#include "MathUtility.h"

int NumberLoop(int num, int max, int min)
{
	if (num > max) { return num - max + min; }
	if (num < min) { return max; }
	return num;
}