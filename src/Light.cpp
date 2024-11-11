#include "Light.h"


Light::Light() : m_VAO(), m_VBO(LightVertices, sizeof(LightVertices)), m_EBO(LightIndices, sizeof(LightIndices))
{
	m_VAO.Bind();
	m_VBO.Bind();

	m_VAOS.push_back(m_VAO.GetID());
	m_VBOS.push_back(m_VBO.GetID());
	m_VBOS.push_back(m_EBO.GetID());

	m_VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, sizeof(float) * 3, (void*)0);

	m_EBO.Bind();

	m_VAO.Unbind();
	m_EBO.Unbind();
	m_VBO.Unbind();



}


void Light::Render()
{
	m_VAO.Bind();
	m_EBO.Bind();
	glDrawElements(GL_TRIANGLES, sizeof(LightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}