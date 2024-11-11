#include"VAO.h"

// Constructor that generates a VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &m_ID);
	Bind();
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_ID);
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() const
{
	glBindVertexArray(m_ID);
}

// Unbinds the VAO
void VAO::Unbind() const
{
	glBindVertexArray(0);
}

