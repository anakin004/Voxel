#ifndef RAY_H
#define RAY_H

#include<glm/glm.hpp>

class Ray{

public:
	
	Ray(glm::vec3 start, glm::vec3 ori);
	glm::vec3& march();


private:

	glm::vec3 m_Pos;
	glm::vec3 m_Ori;


};






#endif
