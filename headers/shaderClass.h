#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <unordered_map>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util.h"



struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


ShaderProgramSource get_file_contents(const std::string& filename);

class Shader
{

	public:
		GLuint m_ID;

		Shader(const std::string& filename);
		static void DeleteShaders();
		static void InitShaders();

		static void BindShaderID(GLint ID);
		void SetUniform1f(const std::string& uni_name, const float val);
		void SetUniform1i(const std::string& uni_name, const GLint val);
		void SetUniformMat4f(const std::string& uni_name, const glm::mat4& matrix);
		void SetUniformVec3f(const std::string& uni_name, const glm::vec3& vector);
		void SetUniformVec4f(const std::string& uni_name, const glm::vec4& vector);
		void SetUniform3fs(const std::string& uni_name, const glm::vec3 vector[6]);

		GLint getUniform(const std::string& uni_name);
		static Shader* getShader(const std::string& shader_name);
			
		void Bind() const;
		void Unbind() const;
		void compileErrors(unsigned int shader, const char* type);

		inline GLint GetID() { return m_ID; };

	private:
		std::unordered_map<std::string, GLint> m_UniformLocationCache;
		static std::unordered_map<std::string, Shader*> m_ShaderLocationCache;
};

#endif