#include "shaderClass.h"
#include <glm/gtc/type_ptr.hpp> // for value_ptr




std::unordered_map<std::string, Shader*> Shader::m_ShaderLocationCache = {};

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const std::string& filepath) : m_UniformLocationCache()
{
	// Read vertexFile and fragmentFile 
	ShaderProgramSource shaderCode = get_file_contents(filepath);

	// Convert the shader source strings into character arrays
	const char* vertexSource = shaderCode.VertexSource.c_str();
	const char* fragmentSource = shaderCode.FragmentSource.c_str();



	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// Create Shader Program Object and get its reference
	m_ID = glCreateProgram();

	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);
	compileErrors(vertexShader, "PROGRAM");

	glLinkProgram(m_ID);
	glValidateProgram(m_ID);


	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// Reads a text file and outputs a string with everything in the text file
ShaderProgramSource get_file_contents(const std::string& filename)
{
	std::ifstream in(filename.c_str(), std::ios::binary);
	

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	// 1 for vert and 1 for frag
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	if (in)
	{
		std::string line;

		while (getline(in, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;

				}
			}
			
			else {
				ss[(int)type] << line << "\n";
			}
		}

		return { ss[0].str() , ss[1].str() };
	}
	throw(errno);
}


void Shader::SetUniformMat4f(const std::string& uni_name, const glm::mat4& matrix)
{
	// obv id, then num of matrix's which is 1, then whether or not we have a row
	// major matrix or column to transpose, since its col we dont
	glUniformMatrix4fv(getUniform(uni_name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniform3fs(const std::string& uni_name, const glm::vec3 vector[6])
{
	glUniform3fv(getUniform(uni_name), 6, glm::value_ptr(vector[0]));
}


void Shader::InitShaders()
{
	Shader* baseShader = new Shader("resources/base.shader");
	m_ShaderLocationCache["base.shader"] = baseShader;

	baseShader->Bind();
	baseShader->SetUniform1f("u_Texture", 0);
	baseShader->Unbind();

	// constructing the light source
	Shader* lightShader = new Shader("resources/light.shader");
	m_ShaderLocationCache["light.shader"] = lightShader;

	lightShader->Bind();

	// might seem out of place, but its alot easier just to initialize lighting
	// here and loading into shader
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = { 0 , 200 , 0 };
	glm::vec3 lightNorm = glm::normalize(lightPos);
	glm::mat4 lightModelMatrix = glm::translate(glm::mat4(1.0f), lightPos);

	lightShader->SetUniformVec4f("u_lightColor", glm::vec4(lightColor, 1.0f));
	lightShader->SetUniformMat4f("u_modelMatrix", lightModelMatrix);


	baseShader->Bind();
	baseShader->SetUniformVec3f("u_lightColor", lightColor);
	baseShader->SetUniformVec3f("u_lightPos", lightPos);
	baseShader->SetUniformVec3f("u_lightNorm", lightNorm);
	baseShader->SetUniform3fs("u_faceNorms", FaceNormals);


}


void Shader::BindShaderID(GLint ID)
{
	glUseProgram(ID);
}


// Activates the Shader Program
void Shader::Bind() const
{
	glUseProgram(m_ID);
}
void Shader::Unbind() const
{
	glUseProgram(0);
}

// Deletes the Shader Programs
void Shader::DeleteShaders()
{
	for (auto& it : m_ShaderLocationCache) {
		glDeleteProgram(it.second->GetID());
		delete it.second;
	}
}


GLint Shader::getUniform(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	
	GLint loc = glGetUniformLocation(m_ID, name.c_str());
	if (loc == -1 && name == "u_faceNorms")
		std::cout << "Warning: uniform: " << name << " does not exist" << std::endl; 
	else
		m_UniformLocationCache[name] = loc;

	return loc;
}

Shader* Shader::getShader(const std::string& name)
{
	if (m_ShaderLocationCache.find(name) != m_ShaderLocationCache.end())
		return m_ShaderLocationCache[name];

	else
		std::cout << "Couldnt find shader name ERROR ERROR ";
	

	return nullptr;
}

void Shader::SetUniformVec4f(const std::string& uni_name, const glm::vec4& vector)
{
	glUniform4f(getUniform(uni_name), vector.x, vector.y, vector.z, vector.w);

}

void Shader::SetUniformVec3f(const std::string& uni_name, const glm::vec3& vector)
{
	glUniform3f(getUniform(uni_name), vector.x, vector.y, vector.z);

}

void Shader::SetUniform1f(const std::string& uni_name, const float val)
{
	glUniform1f(getUniform(uni_name), val);
}

void Shader::SetUniform1i(const std::string& uni_name, const int val)
{
	glUniform1i(getUniform(uni_name), val);
}


// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}

