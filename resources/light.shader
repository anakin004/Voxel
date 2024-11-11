#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_modelMatrix;
uniform mat4 u_camMatrix;

void main()
{
	gl_Position = u_camMatrix * u_modelMatrix * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;

uniform vec4 u_lightColor;

void main()
{
	FragColor = u_lightColor;
}