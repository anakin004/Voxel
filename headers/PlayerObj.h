#ifndef PLAYER_H
#define PLAYER_H

#include "World.h"
#include<glm/glm.hpp>



// this class primarily serves as a medium between the World and the user
// -- more so, accounting for multiplayer funtionality, I wanted a class that is able to access chunk positions
// and world attributes, but doesnt have things like a camera, and positional calculations like collisions
// since ... the positional data - x,y,z , rotation , etc, will be sent by udp packages
// there is a user class (User.cpp/User.h) that represents the self

class PlayerObj : public World {

public:
	




protected:

	glm::vec3 m_Position;
	glm::vec3 m_Orientation;


};




#endif
