#include "Camera.h"
#include "World.h"
#include "Ray.h"
#include "Math.h"

const float GRAVITY = -9.81f * 2.7; // amplifying gravity
const float TERMINAL_VELOCITY = -50.0f;
const float fixedDeltaTime = 1.0f / 144.0f; // 144 updates per second (16.66 ms per update)


Camera::Camera(int width, int height, glm::vec3& position, float fov, float nearPlane, float farPlane)
	: m_Width(width), m_Height(height), m_Position(position), m_MVP(glm::mat4(1.0f)), m_Orientation(0.0f, 0.0f, -1.0f)
{
	m_ProjectionMat = glm::perspective(glm::radians(fov), float(m_Width) / float(m_Height), nearPlane, farPlane);
}

void Camera::PushMVP(Shader* shader, const std::string& uniform)
{
	shader->SetUniformMat4f(uniform, m_MVP);
}

void Camera::SetMVP( glm::mat4& modelMat)
{

	glm::mat4 view = glm::lookAt(m_Position, m_Orientation + m_Position, m_Up);
	m_MVP = m_ProjectionMat * view * modelMat;

}


void Camera::Inputs(GLFWwindow* window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 xzDir(0.0f);
		xzDir.x = m_Orientation.x;
		xzDir.z = m_Orientation.z;
		xzDir = glm::normalize(xzDir);
		m_Position += m_Speed * xzDir;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Position += m_Speed * -glm::normalize(glm::cross(m_Orientation, m_Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 xzDir(0.0f);
		xzDir.x = m_Orientation.x;
		xzDir.z = m_Orientation.z;
		xzDir = glm::normalize(xzDir);
		m_Position += m_Speed * -xzDir;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Position += m_Speed * glm::normalize(glm::cross(m_Orientation, m_Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS /*&& m_Grounded*/)
	{
		m_CurAirVel = m_BaseJumpVel;  // Set initial jump velocity
		m_Position.y += m_Speed;  // for debuggin, commet out later
		m_Jumped = true;              // Mark as jumped
		m_Grounded = false;           // Set grounded to false
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_Position += m_Speed * -m_Up;
	}




	/*
	* quick side note
	* i know that since many of the globals like chunk_size , height ,etc
	* are a power of two
	* and i can do shifting
	* but the compiler should be doing optimizations(shifting) since they are constants and can be detemined at compile time
	*/

	


	int chunkX = (int)std::floor(m_Position.x / CHUNK_SIZE);
	int chunkZ = (int)std::floor(m_Position.z / CHUNK_SIZE);

	
	Chunk* curChunk = World::GetChunk(chunkX, chunkZ);

	if (curChunk != nullptr)
	{
		int* heightMap = curChunk->GetHeights();

		// edge case when our chunk x and/or z is 0, we will have -1 

		int chunkIndexX = std::max(0,(int)m_Position.x - chunkX * CHUNK_SIZE - 1);
		int chunkIndexZ = std::max(0,(int)m_Position.z - chunkZ * CHUNK_SIZE - 1);
		

		// Get the ground level from the height map
		int groundLevel = std::max(0, heightMap[chunkIndexX * CHUNK_SIZE + chunkIndexZ]) + (-MAX_DEPTH);

		//case when we step off, where we havent jumped and we were grounded
		if (m_Position.y > groundLevel && !m_Jumped && m_Grounded)
		{
			m_Grounded = false;
		}


		if (!m_Grounded) {
			// apply gravity to the current air velocity
			m_CurAirVel += GRAVITY * fixedDeltaTime;

			// clamp the downward velocity to prevent unrealistic falls
			if (m_CurAirVel < TERMINAL_VELOCITY) {
				m_CurAirVel = TERMINAL_VELOCITY;
			}

			// Update the player's vertical position based on the current air velocity
			m_Position.y += m_CurAirVel * fixedDeltaTime;


			// Check if the player has hit the ground, 3.0 is the camera offset from ground
			if (m_Position.y <= groundLevel + 3.0f) { 
				m_Position.y = groundLevel + 3.0f; 
				m_CurAirVel = 0.0f;            
				m_Grounded = true;               
				m_Jumped = false;                 
			}
		}
		else {
			// Reset vertical velocity when grounded (optional)
			m_CurAirVel = 0.0f;  // Ensure no upward force if grounded
		}
	}
	

	// PAUSING
	if (!m_Paused && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_Paused = true;
	}

	// UNPAUSING
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		m_Paused = false;
	}
	

	if( !m_Paused )
	{

		
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			Ray newRay = Ray(m_Position, m_Orientation);
			bool hit = false;
			bool first = true;
			static float timeHeld = 0;

			static double lastHit = -2;
			if (glfwGetTime() - lastHit > .5) {
				lastHit = glfwGetTime(); // Always update time when the loop is entered
				while (!hit) {
					const glm::vec3 cur = newRay.march();

					if (vec3Dist(cur, m_Position) >= 4) break;

					int cx = (int)std::floor(cur.x / 16.f);
					int cz = (int)std::floor(cur.z / 16.f);
					int x = (int)std::floor(cur.x - 16 * cx);
					int y = (int)std::floor(cur.y);
					int z = (int)std::floor(cur.z - 16 * cz);
					Chunk* c = World::GetChunk(cx, cz);

					hit = c->CheckBit(x, y, z);
					std::cout << y;
					if (hit) {
						//std::cout << "Player Pos is: " << m_Position.x << " " << m_Position.y << std::endl;
						c->RemoveBlock(x, y, z);
					}
				}
			}
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float rotX = m_Sensitivity * (float)(mouseY - (m_Height / 2)) / m_Height;
		float rotY = m_Sensitivity * (float)(mouseX - (m_Width / 2)) / m_Width;

		m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);
		glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_Orientation, m_Up)));

		if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			m_Orientation = newOrientation;
		}

		glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


	}		


}
