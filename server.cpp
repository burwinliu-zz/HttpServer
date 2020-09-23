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
    std::string response;


    std::cout << "Server started on port " << pPortNumber << "." <<std::endl;
    pNetworkHelper.Listen();
    while(pNetworkHelper.getSetupSuccess()){
        clientSock = pNetworkHelper.Accept(NULL, NULL);
        if (clientSock == INVALID_SOCKET){
            std::cout << "Bad socket setup" <<std::endl;
            Sleep(8000);
            return;
        }
        response = pNetworkHelper.Receive(clientSock);

        parseHeader(response);


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

    std::string header;
    int firstPos, secondPos;
    
    if (pResponse == ""){
        return result;
    }
    std::cout << pResponse << std::endl;
    header = pResponse.substr(0, pResponse.find("\n"));
    std::cout << "# RECIEVED " << header << " w/ " << pResponse.find("\n") << "-" << std::endl;

    printf("# RECIEVED %s w/ %d\n", header.c_str(), pResponse.find("\n"));

    firstPos = header.find(" ");
    secondPos = header.find(" ", firstPos+1);
    result.requestType = header;
    result.requestPath = header;
    result.requestProtocol = header;

    result.requestType = result.requestType.substr(0, firstPos);
    result.requestPath = result.requestPath.substr(firstPos+1, (secondPos-firstPos) - 1);
    result.requestProtocol = result.requestProtocol.substr(secondPos+1, result.requestProtocol.length() - secondPos - 1);

    return result;
}