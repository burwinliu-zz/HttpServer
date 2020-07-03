#include "server.h"


HttpServer::HttpServer(){
    mPortNumber = (std::string) "8080";
    mIResult = WSAStartup(MAKEWORD(2,2), &mWsaData);
    try{
        initializeSocket();
    }
    catch(std::string e){
        std::cout << e << std::endl;
    }

}

HttpServer::HttpServer(int pPortNumber){
    mPortNumber = pPortNumber;
}

HttpServer::~HttpServer(){
    return;
}

std::string HttpServer::fileToString(std::string pFilePath){
    return "";
}

void HttpServer::initializeSocket(){
    mIResult = WSAStartup(MAKEWORD(2,2), &mWsaData);
    if (mResult != 0) {
        printf("WSAStartup failed with error: %d\n", mResult);
        return;
    }
    ZeroMemory(&mHints, sizeof(mHints));
    mHints.ai_family = AF_INET;
    mHints.ai_socktype = SOCK_STREAM;
    mHints.ai_protocol = IPPROTO_TCP;
    mHints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    mIResult = getaddrinfo(NULL, mPortNumber.c_str(), &mHints, &mResult);
    if ( mIResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", mIResult);
        WSACleanup();
        return;
    }
    // Create a SOCKET for connecting to server
    mListenSocket = socket(mResult->ai_family, mResult->ai_socktype, mResult->ai_protocol);
    if (mListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(mResult);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    mIResult = bind( mListenSocket, mResult->ai_addr, (int)mResult->ai_addrlen);
    if (mIResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(mResult);
        closesocket(mListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(mResult);

    mIResult = listen(mListenSocket, SOMAXCONN);
    if (mIResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(mListenSocket);
        WSACleanup();
        return;
    }
}
