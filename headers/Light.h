#ifndef LIGHT_H
#define LIGHT_H

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "util.h"
#include "Object.h"
#include "Render.h"
#include "shaderClass.h"

class Light : private Object
{
public:

	Light();

	void Render();

private:

	VAO m_VAO;
	VBO m_VBO;
	EBO m_EBO;
};



#endif