#include "server.h"

HttpServer::HttpServer(int pPortNumber){
    WSADATA wsaData;
    int iResult;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return;
    }

    pNetworkHelper = SocketWrapper(pPortNumber);
}

HttpServer::~HttpServer(){
    WSACleanup();
    return;
}

int HttpServer::SetResource(std::string pPathName, std::string pRequestType, std::string pResourcePath){
    //TODO COMPLETE
    if(mMapResources.count(pPathName) == 0){
        std::map<std::string, std::string> map;
        mMapResources[pPathName] = map;
        mMapResources[pPathName][pRequestType] = pResourcePath;
        return 0;
    }
    else if(mMapResources[pPathName].count(pRequestType) == 0){
        mMapResources[pPathName][pRequestType] = pResourcePath;
        return 0;
    }
    else{
        return -1;
    }
}

void HttpServer::StartServer(){
    SOCKET client;
    SocketWrapper net = SocketWrapper();
    while(1){
        client = net.Accept(NULL, NULL);
        std::cout << net.Recieve(client);
        net.Send("TESTING\n\0", client);
        net.Cleanup(client); 
    }
}

std::string HttpServer::fileToResponse(std::string pFilePath){
    return "";
}

std::string HttpServer::fileToString(std::string pFilePath){
    return "";
}

HttpServer::RequestInfo HttpServer::parseHeader(std::string pResponse){
    struct RequestInfo result;
    result.requestPath = "";
    result.requestProtocol ="";
    result.requestType = "";
    return result;
}