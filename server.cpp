#include "server.h"

HttpServer::HttpServer(int portNum){
    pPortNumber = portNum;
    pNetworkHelper = SocketWrapper(pPortNumber);
}

HttpServer::~HttpServer(){
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
    SOCKET clientSock;

    std::cout << "SERVER STARTED ON " << pPortNumber << "." <<std::endl;
    while(pNetworkHelper.getSetupSuccess()){
        clientSock = pNetworkHelper.Accept(NULL, NULL);
        if (clientSock == INVALID_SOCKET){
            std::cout << "INVALID SOCKET." <<std::endl;
            Sleep(8000);
            return;
        }
        pNetworkHelper.Send("TESTING\n\0", clientSock);
        pNetworkHelper.Cleanup(clientSock); 
        Sleep(8000);
    }
    Sleep(8000);
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