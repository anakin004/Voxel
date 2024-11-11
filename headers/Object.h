#ifndef OBJECT_H
#define OBJECT_H


#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include <vector>

class Object
{
public:
	Object();

	static void DestroyObjects();

protected:

	static std::vector<GLuint> m_VBOS;
	static std::vector<GLuint> m_VAOS;
	static std::vector<GLuint> m_EBOS;
};





#endif