#ifndef HTTPSERVER_SOCKETWRAPPER_H_
#define HTTPSERVER_SOCKETWRAPPER_H_

#include <string>
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <unordered_map>
#include <sstream> 


#pragma comment (lib, "Ws2_32.lib")

class SocketWrapper{
    public:
        // Constructs/Destructors
        SocketWrapper(int portNum=8000);
        ~SocketWrapper();  

        SOCKET Accept(sockaddr *addr, int *addrlen);
        int Send(std::string message, SOCKET clientSocket);
        std::string Recieve(SOCKET clientSocket);
        void Cleanup(SOCKET socket);

        int getSetupSuccess();

    private:
        // Class variables
        // For Socket Setup
        WSADATA mWsaData;

        std::string mPortNumber;
        int mIResult;
        int mSendResult;

        SOCKET mListenSocket = INVALID_SOCKET;

        struct addrinfo *mResult;
        struct addrinfo mHints;

        //For Invalid Sockets
        int mSetupSuccess = 0;
        int mSocketBound = 0;   


};


#endif