#ifndef NETWORK_H
#define NETWORK_H

#include <SDL2/SDL_net.h>


// could use glm::vec3 but it has some overhead
struct vector {
    float x, y, z;
};

struct userData {
    vector pos;
    vector ori;
};


struct responseData{
    int pID;
    UDPsocket udpSocket;
    TCPsocket tcpSocket;
};


void connectToServer(const char* ip, responseData &r );
void receivePlayerStates(UDPsocket clientSocket);
void testSendUDP( UDPsocket udpSocket);


extern const char* HOST;



#endif