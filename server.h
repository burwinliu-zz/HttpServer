#ifndef HTTPSERVER_SERVER_H_
#define HTTPSERVER_SERVER_H_

#include <string>
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <sstream> 
#include <map>
#include "socketWrapper.h"

#include <Windows.h>


#pragma comment (lib, "Ws2_32.lib")

class HttpServer{
    public:
        // Constructs/Destructors
        HttpServer(int pPortNumber=8080);
        ~HttpServer();  


        // Methods and functions
        int SetResource(std::string pPathName, std::string pRequestType, std::string pResourcePath);
        void StartServer();


        //Structs
        struct PageInfo{
            std::string requestType;
            std::string requestPath;
        };

        struct RequestInfo{
            std::string requestType;
            std::string requestPath;
            std::string requestProtocol;
        };

    private:
        // Class variables
        // For Socket Setup
        SocketWrapper pNetworkHelper;
        int pPortNumber;

        //For serving resources to paths
        std::map<std::string, std::map<std::string, std::string>> mMapResources;


        //Helpers
        std::string fileToResponse(std::string pFilePath);
        std::string fileToString(std::string pFilePath);
        struct RequestInfo parseHeader(std::string pResponse);
};


#endif