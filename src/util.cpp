#include "util.h"


const verts BaseBlock[24] =
{
	// front
	{ {0.0f, 0.0f, 1.0f}, {0, 0} ,0 },// bottom left       
	{ {1.0f, 0.0f, 1.0f}, {0, 0} ,0},  // bottom right
	{ {0.0f, 1.0f, 1.0f}, {0, 0} ,0},  // top left
	{ {1.0f, 1.0f, 1.0f}, {0, 0} ,0},  // top right

	// left side
	{ {0.0f, 0.0f, 0.0f}, {0, 0} ,1},  // bottom left       
	{ {0.0f, 0.0f, 1.0f}, {0, 0} ,1},  // bottom right
	{ {0.0f, 1.0f, 0.0f}, {0, 0} ,1},   // top left
	{ {0.0f, 1.0f, 1.0f}, {0, 0} ,1},   // top right

	// back side
	{ {1.0f, 0.0f, 0.0f}, {0, 0} ,2},// bottom left       
	{ {0.0f, 0.0f, 0.0f}, {0, 0} ,2},  // bottom right
	{ {1.0f, 1.0f, 0.0f}, {0, 0} ,2},  // top left
	{ {0.0f, 1.0f, 0.0f}, {0, 0} ,2},  // top right

	// right side
	{ {1.0f, 0.0f, 1.0f}, {0, 0} ,3},  // bottom left       
	{ {1.0f, 0.0f, 0.0f}, {0, 0} ,3},   // bottom right
	{ {1.0f, 1.0f, 1.0f}, {0, 0} ,3},  // top left
	{ {1.0f, 1.0f, 0.0f}, {0, 0} ,3}, // top right

	// bottom
	{ {0.0f, 0.0f, 1.0f}, {0, 0} ,4},  // bottom left
	{ {1.0f, 0.0f, 1.0f}, {0, 0} ,4},  // bottom right
	{ {0.0f, 0.0f, 0.0f}, {0, 0} ,4},  // top left
	{ {1.0f, 0.0f, 0.0f}, {0, 0} ,4},  // top right

	// top
	{ {0.0f, 1.0f, 1.0f}, {0, 0} ,5},   // bottom left
	{ {1.0f, 1.0f, 1.0f}, {0, 0} ,5},   // bottom right
	{ {0.0f, 1.0f, 0.0f}, {0, 0} ,5},  // top left
	{ {1.0f, 1.0f, 0.0f}, {0, 0} ,5}, // top right

};

const glm::vec3 FaceNormals[6] = {
	{0.0f, 0.0f, 1.0f},   // front
	{-1.0f, 0.0f, 0.0f},  // left
	{0.0f, 0.0f, -1.0f},  // back
	{1.0f, 0.0f, 0.0f},   // right
	{0.0f, -1.0f, 0.0f},  // bottom
	{0.0f, 1.0f, 0.0f}    // top
};


// Indices for vertices order
const indexCoords BaseBlockIndices[12] =
{
	{0, 1, 2},  // f. left tri
	{1, 2, 3},  // f. right tri
	{4, 5, 6},  // ls. left tri
	{5, 6, 7},  // ls. right tri
	{8, 9, 10},  // ba. left tri
	{9, 10, 11},  // ba. right tri
	{12, 13, 14},  // rs. left tri
	{13, 14, 15},  // rs. right tri
	{16, 17, 18},  // bottom. left tri
	{17, 18, 19},  // bottom. right tri
	{20, 21, 22},  // top. left tri
	{21, 22, 23},  // top. right tri
};


const float LightVertices[24] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	0.1f,  0.1f, -0.1f,
	0.1f,  0.1f,  0.1f
};

const GLuint LightIndices[36] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


