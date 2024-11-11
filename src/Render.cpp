#include "Render.h"
#include "Debug.h"



void Renderer::Clear() const
{
	// Specify the color of the background
	glClearColor(0.494f, 0.753f, 0.933f, 0.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::Draw(const VAO& va, Shader& shader, GLuint numIndices)
{
	// acitvate and get uniform can both happen above while loop since when we activate it we dont need
	// to keep activating it, as it stays in use until otherwise specified
	// Tell OpenGL which Shader Program we want to use

	shader.Bind();
	va.Bind();

	GlCall(glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0));

}

