#include "network.h"
#include <string.h>
#include <cstring>

const Uint16 DST_UDP_PORT = 12345;
const Uint16 SRC_UDP_PORT = 12347;
const Uint16 DST_TCP_PORT = 12344;
const Uint16 SRC_TCP_PORT = 12346;

// redacted for obvious reason
// if you want to play this yourself you put the hosts ip - ec. whereever the server is being hosted from
const char* HOST = "";

// TCP handshake
bool performTCPHandshake(const char* ip, responseData &r ) {
    

    bool success = true;
    TCPsocket tcpSocket = nullptr;
    IPaddress ipaddr;
    int playerID = -1;

    if (SDLNet_ResolveHost(&ipaddr, ip, DST_TCP_PORT) < 0) {
        SDL_Log("Failed to resolve server address: %s", SDLNet_GetError());
        success = false;
    }

    else{
        tcpSocket = SDLNet_TCP_Open(&ipaddr);
        
        if (!tcpSocket) {
            SDL_Log("Failed to open TCP socket: %s", SDLNet_GetError());
            success = false;
        }
        else{
            bool flag = true;
            if (SDLNet_TCP_Send(tcpSocket, &flag ,  sizeof(bool))  <  sizeof(bool) ) {
                SDL_Log("Failed to send handshake message: %s", SDLNet_GetError());
                SDLNet_TCP_Close(tcpSocket);
                success = false;
            }

            else{
                if (SDLNet_TCP_Recv(tcpSocket, &playerID, sizeof(int) ) < sizeof(int ) ) {
                    SDL_Log("Failed to receive response: %s, playerID is still -1", SDLNet_GetError());
                    SDLNet_TCP_Close(tcpSocket);
                    success = false;
                }
            }

        }
    }

    r.pID = playerID;
    r.tcpSocket = tcpSocket;

    SDL_Log("Received response: pID is %d", playerID);
    return true;
}

void testSendUDP(UDPsocket udpSocket) {
    UDPpacket *packet = SDLNet_AllocPacket(sizeof(bool));  
    if (!packet) {
        SDL_Log("Failed to allocate UDP packet: %s", SDLNet_GetError());
        return;
    }

    IPaddress dest_ip;
    if (SDLNet_ResolveHost(&dest_ip, HOST , DST_UDP_PORT) == -1) {
        SDL_Log("Failed to resolve destination host: %s", SDLNet_GetError());
        return;
    }

    packet->address = dest_ip;  // setting destination address and port
    bool value = true;  
    memcpy(packet->data, &value, sizeof(bool)); \
    packet->len = sizeof(bool);  \

    if (SDLNet_UDP_Send(udpSocket, -1, packet) == 0) {
        SDL_Log("Failed to send UDP packet: %s", SDLNet_GetError());
    } else {
        SDL_Log("Packet sent successfully");
    }

    SDLNet_FreePacket(packet);  \
}


// Switch to UDP
void connectToServer(const char* ip, responseData &r) {

    if ( !performTCPHandshake(ip, r) ) {
        SDL_Log("COudlnt preform tcp");
    }

    else {

        IPaddress ipaddr;

        if (SDLNet_ResolveHost(&ipaddr, ip, SRC_UDP_PORT) < 0) 
            SDL_Log("Failed to resolve server address: %s", SDLNet_GetError());
        
        else{
            UDPsocket udpSocket = SDLNet_UDP_Open(SRC_UDP_PORT); // we can choose port if irts not in use / valid port

            if (!udpSocket) 
                SDL_Log("Failed to open UDP socket: %s", SDLNet_GetError());
            
            else{
                r.udpSocket = udpSocket;
            }

        }
    
    }

  
}



void receivePlayerStates(UDPsocket udpSocket) {
    UDPpacket* packet = SDLNet_AllocPacket(sizeof(userData));

    if (SDLNet_UDP_Recv(udpSocket, packet)) {
        userData state;
        if (packet->len == sizeof(userData)) {
            memcpy(&state, packet->data, sizeof(state));

        } else {
            SDL_Log("Incomplete data received");
        }
    } else {
        SDL_Log("recv() failed: %s", SDLNet_GetError());
    }

    // Clean up
    SDLNet_FreePacket( packet );

}
