#include"VBO.h"



VBO::VBO()
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	GlCall(glBufferData(GL_ARRAY_BUFFER, sizeof(BaseBlock) * CHUNK_SIZE * CHUNK_SIZE * MAX_LEVELS, nullptr, GL_DYNAMIC_DRAW));
}


void VBO::LoadBuffer(verts* vertices, GLuint numVerts)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	GlCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts) * numVerts, vertices));

}

// Deletes the VBO
VBO::~VBO()
{
	glDeleteBuffers(1, &m_ID);
}

// Binds the VBO
void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

// Unbinds the VBO
void VBO::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}