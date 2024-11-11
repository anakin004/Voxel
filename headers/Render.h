#ifndef RENDERER_H
#define RENDERER_H



#include "VAO.h"
#include "EBO.h"
#include "shaderClass.h"

class Renderer
{

public:


	void Draw(const VAO& va, Shader& shader, GLuint numIndices);
	void Clear() const;


private:

};














#endif