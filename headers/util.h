#ifndef UTIL_H
#define UTIL_H

#include <GLFW/glfw3.h>
#include "Block.h"
#include <glm/glm.hpp>
#include <array>



//DIRECTION
enum Direction {
	NORTH = 0,
	WEST,
	SOUTH,
	EAST,
	DOWN,
	UP,
};

enum Corner {
	BOTTOM_LEFT = 0,
	BOTTOM_RIGHT,
	TOP_LEFT,
	TOP_RIGHT,
};

constexpr normal DIRECTION_VEC[6] = {
	{  0,  0,  1 },
	{ -1,  0,  0 },
	{  0,  0, -1 },
	{  1,  0,  0 },
	{  0, -1,  0 },
	{  0,  1,  0 }
};


extern const verts BaseBlock[24];
extern const indexCoords BaseBlockIndices[12];
extern const float LightVertices[24];
extern const GLuint LightIndices[36];
extern const glm::vec3 FaceNormals[6];


constexpr int MAX_DEPTH = -16;
constexpr int MAX_LEVELS = 64;
constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_HEIGHT = 256;



#endif 
