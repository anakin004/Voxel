#include "EBO.h"



EBO::EBO()
{

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(BaseBlockIndices) * CHUNK_SIZE * CHUNK_SIZE * MAX_LEVELS, nullptr , GL_DYNAMIC_DRAW));

}


void EBO::LoadBuffer(indexCoords* indices, GLuint numIndices) 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	GlCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indexCoords) * numIndices , indices));

}

// Deletes the EBO
EBO::~EBO()
{
	glDeleteBuffers(1, &m_ID);
}

// Binds the EBO
void EBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

// Unbinds the EBO
void EBO::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

