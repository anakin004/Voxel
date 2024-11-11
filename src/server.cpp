#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <SDL2/SDL_net.h>
#include <thread>

// if you are to use this code and it doesnt work, check the firewall and see if these ports are unopened and/or restriced
const Uint16 SRC_UDP_PORT = 12345;
const Uint16 DST_UDP_PORT = 12347;
const Uint16 SRC_TCP_PORT = 12344;
const Uint16 DST_TCP_PORT = 12346;
const Uint8 MAX_PLAYERS = 4;

struct playerState{
        int x;
        int y;
        double rotation;
        int pID;
        bool shot;
        Uint8 idx;
        bool resetting;
};


std::vector<IPaddress> clientIPS;
std::vector<playerState> globalPlayers;
std::vector<std::thread> playerThreads;
SDL_mutex* gameMutex = nullptr;  
bool gGameStarted = false;

void printIP(const IPaddress& ip) {
    // Extract the IP address and port
    Uint32 ipAddress = SDLNet_Read32(&ip.host);
    Uint16 port = SDLNet_Read16(&ip.port);

    // Convert the IP address to a human-readable format (dot-decimal)
    SDL_Log("IP: %d.%d.%d.%d, Port: %u",
            (ipAddress >> 24) & 0xFF,
            (ipAddress >> 16) & 0xFF,
            (ipAddress >> 8) & 0xFF,
            ipAddress & 0xFF,
            port);
}

bool initNetworking() {
    if (SDLNet_Init() < 0) {
        SDL_Log("Failed to initialize SDL_net: %s", SDLNet_GetError());
        return false;
    }
    gameMutex = SDL_CreateMutex();
    if (gameMutex == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create mutex: %s", SDL_GetError());
        return false;
        }
    return true;
}

/*            
   precondition: game has started / all players connected
   and that we have put all ips into clientIPS
*/
void handlePacket(UDPpacket* packet, UDPsocket udpSocket) {

    /*
     * Player State is stored in packet, so we only need to reinterpret
     * cast the packet to a playerState pointer so we can check the ip that sent it
     * so we are not sending the same packet back to that person
     */
    
    playerState* gameState = reinterpret_cast<playerState*>(packet->data);
    IPaddress* senderIP = &packet->address;

    // Broadcast the state to all other clients
    for (int i = 0; i < clientIPS.size(); ++i) {
        if ( gameState->pID != i ) { 
            
            // setting packet address to each different connect player
            //  that is not the player who sent the package
            // keep in mind when we send the notice the game started we changed the clientIPS dest port to the universal
            // destination udp port

            packet->len = sizeof(playerState);
            packet->address = clientIPS[i];
            SDLNet_UDP_Send(udpSocket, -1, packet);


        }
    }

}


void sendNotice( TCPsocket clientSocket ) {

    SDL_LockMutex( gameMutex );

    while( !gGameStarted ){
        SDL_Delay(100);
    }

    bool flag = true;
    if( SDLNet_TCP_Send( clientSocket, &flag, sizeof(flag) ) > 0 ){
        SDL_Log("Player Joined Game");
    }

    //unlock after we are done 
    SDL_UnlockMutex(gameMutex);

}


// returns false if a player has already connected, true if they havent
int handleTCPClient(TCPsocket clientSocket) {
    bool flag;
    int playerID = 0;


    // tcp stuff isnt thread safe, and we access globals so just lock all since constant locking and unlocking
    // can be expensive

    SDL_LockMutex( gameMutex );


    if (SDLNet_TCP_Recv(clientSocket, &flag, sizeof(bool)) > 0) {
        IPaddress clientAddress;
        memcpy( &clientAddress, SDLNet_TCP_GetPeerAddress(clientSocket), sizeof(IPaddress) );

        int uniqueClient = 1;
        for(short i = 0 ; i < clientIPS.size(); i++){
            if( clientIPS[i].host == clientAddress.host ){
                uniqueClient = 0;
                playerID = i;
                printIP( clientAddress );
                break;
            }
        }


        if( uniqueClient ){
                playerState initPlayer = {0, 0, 0.0, (int)globalPlayers.size(), false, 0, false};
                SDL_Log("Player Connected");
                clientAddress.port = SDL_SwapBE16(DST_UDP_PORT);
                printIP(clientAddress);
                playerID = globalPlayers.size();
                clientIPS.push_back(clientAddress);
                SDL_Log("Number of Players: %d", clientIPS.size());

                SDL_LockMutex( gameMutex );
                globalPlayers.push_back(initPlayer);
        }

        // we want to sent back the playerID
        SDLNet_TCP_Send( clientSocket, &playerID, sizeof(playerID) );

        playerThreads.emplace_back( sendNotice , clientSocket );

        SDL_UnlockMutex( gameMutex );


        return uniqueClient;

    }
        // case when package was unsuccessfuly to be recieved
        // --- no new client
    return 0;
}

/*
void testSendUDP(UDPsocket udpSocket) {
    // Allocate memory for packet
    UDPpacket *packet = SDLNet_AllocPacket(512);
    if (!packet) {
        SDL_Log("Failed to allocate UDP packet: %s", SDLNet_GetError());
        return;
    }

    for( auto &i: clientIPS){
        // network ports are prepresented in little endian
        i.port = SDL_SwapBE16( DST_UDP_PORT );

        packet->address = i;

        // Prepare the message
        const char *message = "Hello";
        size_t message_len = strlen(message);

        memcpy(packet->data, message, message_len);  // Copy data to packet
        packet->len = static_cast<Uint16>(message_len);  // Set packet length

        // Send the packet
        if (SDLNet_UDP_Send(udpSocket, -1, packet) == 0) {
            SDL_Log("Failed to send packet: %s", SDLNet_GetError());
        } else {
            SDL_Log("Sent Packet");
        }

    }

    // Free the allocated packet
    SDLNet_FreePacket(packet);
}

*/

void startServer() {


   IPaddress tcpIP;
   TCPsocket tcpServerSocket, clientSocket;
   if (SDLNet_ResolveHost(&tcpIP, NULL, SRC_TCP_PORT) < 0) {
        SDL_Log("Failed to resolve host: %s", SDLNet_GetError());
        return;
    }

    tcpServerSocket = SDLNet_TCP_Open(&tcpIP);
    if (!tcpServerSocket) {
        SDL_Log("Failed to open TCP server socket: %s", SDLNet_GetError());
        return;
    }

   SDLNet_SocketSet socketSet;
   socketSet = SDLNet_AllocSocketSet( MAX_PLAYERS + 1 );

   if (!socketSet) {
    SDL_Log("Failed to allocate socket set: %s", SDLNet_GetError());
    return;
   }

   if (SDLNet_TCP_AddSocket(socketSet, tcpServerSocket) == -1) {
    SDL_Log("Failed to add socket to set: %s", SDLNet_GetError());
    return;
   }

   
   SDL_Log("TCP Port Opened on %d", SRC_TCP_PORT);
   UDPsocket udpSocket = SDLNet_UDP_Open(SRC_UDP_PORT);

    if (!udpSocket) {
        SDL_Log("Failed to open server socket: %s", SDLNet_GetError());
        return;
    }

    // buff for packet
    UDPpacket* packet = SDLNet_AllocPacket(sizeof(playerState));
    if (!packet) {
        SDL_Log("Failed to allocate packet: %s", SDLNet_GetError());
        SDLNet_UDP_Close(udpSocket);
        SDLNet_Quit();
        return;
    }

    packet->maxlen = 512;

    SDL_Log("UDP Port opened on %d", SRC_UDP_PORT);
    Uint16 receivedLength = 0;

    int numPlayers = 0;
    int minPlayers = 2;

    while (true) {

            if( !gGameStarted ){

                int numReadySockets = SDLNet_CheckSockets(socketSet, 5000); // 5-second timeout


                if (numReadySockets > 0) {

                    if (SDLNet_SocketReady(tcpServerSocket)) {
                        
                        clientSocket = SDLNet_TCP_Accept(tcpServerSocket);
                        
                        if (clientSocket){
                            SDLNet_TCP_AddSocket( socketSet, clientSocket );
                            numPlayers+= handleTCPClient( clientSocket );
                        }
                        
                    }
                }

                // arbitrarily chosen
                if(numPlayers >= minPlayers){
                    
                    gGameStarted = true;

                    // join threads 
                    for( auto &it : playerThreads ){
                        if( it.joinable() )
                            it.join();
                    }
                        
                    SDLNet_TCP_Close( tcpServerSocket );
                    SDLNet_FreeSocketSet( socketSet );
                    SDL_DestroyMutex(gameMutex);

                }


                continue;
            }
            
            // recieve current incoming packet
            receivedLength = SDLNet_UDP_Recv(udpSocket, packet);
            if(receivedLength > 0){
                // at this point the game is started so we can send packts
                handlePacket(packet, udpSocket);
            }
  

        }

    // Clean up
    SDLNet_UDP_Close(udpSocket);
    SDLNet_Quit();
}



/*

int main(int argc, char* argv[]) {
    if ( !initNetworking()) {
        printf("Failed to SDL_net:");
        return -1;
    }

   clientIPS.reserve(MAX_PLAYERS);
   startServer();


    return 0;
}


from make file if you want
g++ -o server src/fighterV2/server.cpp -Iinclude/sdl -lmingw32 -lSDL2main -Llib -lSDL2 -lSDL2_net
./server


*/