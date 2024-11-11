#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include "Block.h"
#include "Debug.h"
#include "util.h"

class VBO
{
private:
	GLuint m_ID;
public:	


	template <typename T>
	VBO(const T vertices[], GLsizei size)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		GlCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
	}

	void LoadBuffer(verts* vertices, GLuint numVerts);

	VBO();

	~VBO();

	void Bind() const;
	void Unbind() const;

	inline GLuint GetID() { return m_ID; };


};


#endif