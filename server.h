#ifndef HTTPSERVER_SERVER_H_
#define HTTPSERVER_SERVER_H_

#include <string>
// #include <windows.h>
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
// #include <stdlib.h>
// #include <stdio.h>


#pragma comment (lib, "Ws2_32.lib")

class HttpServer{
    public:
        // Constructs/Destructors
        HttpServer();
        HttpServer(int pPortNumber);
        ~HttpServer();

    private:
        // Class variables
        WSADATA mWsaData;

        std::string mPortNumber;
        int mIResult;
        int mSendResult;

        SOCKET mListenSocket = INVALID_SOCKET;
        SOCKET mClientSocket = INVALID_SOCKET;

        struct addrinfo *mResult;
        struct addrinfo mHints;

        //Helpers
        std::string fileToString(std::string pFilePath);
        void initializeSocket();



};


#endif