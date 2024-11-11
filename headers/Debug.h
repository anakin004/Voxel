#ifndef DEBUG_H
#define DEBUG_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define ASSERT(x) if (!(x)) __debugbreak();

#define GlCall(x) ClearGl();\
				  x;\
				  ASSERT(GetError(#x, __FILE__, __LINE__ ));



void ClearGl();

bool GetError(const char* function, const char* file, int line);


class Gui
{
public:

	Gui(GLFWwindow*);
	~Gui();
	void HandleGui(glm::vec3& pos);


private:

	GLFWwindow* m_Window;
	ImGuiIO* m_Io;
	
	bool m_show_demo_window = true;
	bool m_show_another_window = false;
	const ImVec4 m_clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);


};


#endif