#include "Debug.h"


void ClearGl()
{
	GLenum err;
	while (err = glGetError());
}

bool GetError(const char* function, const char* file, int line)
{
	GLenum error = glGetError();

	if (error != GL_NO_ERROR)
	{
		std::cout << "[OpenGL Error] : " << error << " line: " << line << ", Function: " << file << " from: " << function << std::endl;
		return false;
	}

	return true;
}


Gui::Gui(GLFWwindow* window) : m_Window(window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_Io = &ImGui::GetIO();
	m_Io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

}

Gui::~Gui()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::HandleGui(glm::vec3& pos)
{

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always); // Position the window

	// Create a named window
	ImGui::Begin("-~+");
	ImGui::Text("x:%.1f\ny:%.1f\nz:%.1f", pos.x, pos.y, pos.z);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_Io->Framerate, m_Io->Framerate);
	ImGui::End();

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(m_Window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
