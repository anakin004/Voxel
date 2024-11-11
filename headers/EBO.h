
#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include "Block.h"
#include "Debug.h"
#include "util.h"

class EBO
{
private:
	// ID reference of Elements Buffer Object
	GLuint m_ID;


public:

	template <typename T>
	EBO(const T indices[], GLsizei size)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW));
	}

	void LoadBuffer(indexCoords* indices, GLuint numIndices);

	EBO();


	// Deletes the EBO
	~EBO();

	// Binds the EBO
	void Bind() const;
	// Unbinds the EBO
	void Unbind() const;

	inline GLuint GetID() { return m_ID; };
	
};

#endif