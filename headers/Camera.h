#ifndef CAMERA_H
#define CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <stdio.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "shaderClass.h"
#include "util.h"

class Camera
{
public:

	Camera(int width, int height, glm::vec3& position, float fov, float nearPlane, float farPlane);


	void PushMVP(Shader* shader, const std::string& uniform);
	void SetMVP( glm::mat4 &modelMat);
	void Inputs(GLFWwindow* window);

	glm::vec3& GetPos() { return m_Position; }

	
private:

	// there is only one up direction, but i decided to tie it into this class for better cache locality
	// because why not
	const glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Position;
	glm::vec3 m_Orientation;
	glm::mat4 m_ProjectionMat;
	glm::mat4 m_MVP;

	int m_Width;
	int m_Height;

	bool m_Jumped = false;
	bool m_Grounded = false;
	bool m_Paused = false;

	// in pixels per second, keep in mind our time step is 144 fps
	float m_CurAirVel = 0.0f;
	const float m_BaseJumpVel = 8.0f;
	const float m_BaseFallVelocity = -5.0f;
	float m_Speed = 0.04f;
	float m_Sensitivity = 100.0f;



};



#endif