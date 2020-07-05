#ifndef HTTPSERVER_NETWORK_H_
#define HTTPSERVER_NETWORK_H_

#include <string>
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <unordered_map>
#include <sstream> 


#pragma comment (lib, "Ws2_32.lib")

class Network{
    public:
        // Constructs/Destructors
        Network(int pPortNumber=8000);
        ~Network();  


        // Methods and functions
        SOCKET bindSocket();

        SOCKET Accept(sockaddr *addr, int *addrlen);
        int Send(std::string message, SOCKET clientSocket);
        std::string Recieve(SOCKET clientSocket);
        void Cleanup(SOCKET socket);

    private:
        // Class variables
        // For Socket Setup

        std::string mPortNumber;
        int mIResult;
        int mSendResult;

        SOCKET mListenSocket = INVALID_SOCKET;

        struct addrinfo *mResult;
        struct addrinfo mHints;

        //For Invalid Sockets
        int mSetupSuccess = 0;

        

        
};


#endif