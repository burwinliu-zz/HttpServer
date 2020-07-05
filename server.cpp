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

    pNetworkHelper = Network(pPortNumber);
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
    //TODO FINISH INNER BLOCK
    SOCKET clientSocket;
    char recvbuf[512];
    int iSendResult;
    int recvbuflen = 512;

    std::string buf;

    

    while(1){
        // Accept a client socket
        clientSocket = pNetworkHelper.Accept(NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            return;
        }

        buf = pNetworkHelper.Recieve(clientSocket);

        std::cout << buf <<std::endl;

        // PARSE HEADER -- IF HEADER IN THE MAP, RETURN THE PAGE, ELSE RETURN ERROR PAGE/HEADER
        struct RequestInfo info;
        info = parseHeader(buf);
        if(strncmp(recvbuf, "GET / HTTP", 10) == 0){
            iSendResult = 0;
            // send( clientSocket, helloPage, mIResult, 0 );
        }
        if (iSendResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
        printf("Bytes sent: %d\n", iSendResult);

        // shutdown the connection since we're done
        pNetworkHelper.Cleanup(clientSocket);
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