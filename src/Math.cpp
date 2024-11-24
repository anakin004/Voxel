#include "Math.h"

double vec3Dist(const glm::vec3& v1, const glm::vec3& v2)
{
	return sqrt(pow(v2.x - v1.x, 2) + pow(v2.z - v1.z, 2) + pow(v2.y - v1.y, 2));
}