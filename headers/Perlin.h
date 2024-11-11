#ifndef PERLIN_H
#define PERLIN_H

#include "noise1234.h"

#include <cmath>


struct vector2 {
	float x, y;
};


float generatePerlinNoise(int worldX, int worldZ);
float perlin(float x, float y);
float interpolate(float a0, float a1, float w);
float dotGridGradient(int ix, int iy, float x, float y);
vector2 randomGradient(int ix, int iy);



#endif