#include "network.h"


// Constructs/Destructors
Network::Network(int pPortNumber){
    int res;
    
    mPortNumber = std::to_string(pPortNumber);

    res = WSAStartup(MAKEWORD(2,2), &mWsaData);
    if (res != 0) {
        printf("WSAStartup failed with error: %d\n", res);
        return;
    }

    ZeroMemory(&mHints, sizeof(mHints));
    mHints.ai_family = AF_INET;
    mHints.ai_socktype = SOCK_STREAM;
    mHints.ai_protocol = IPPROTO_TCP;
    mHints.ai_flags = AI_PASSIVE;
printf("2");
    bindSocket();
}

Network::~Network(){
    // cleanup
    // No longer need server socket
    closesocket(mListenSocket);
    WSACleanup();
}


// Methods and functions
SOCKET Network::Accept(sockaddr *addr, int *addrlen){
    SOCKET clientSocket = accept(mListenSocket, addr, addrlen);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(mListenSocket);
        WSACleanup();
    }
    return clientSocket;
}

int Network::Send(std::string message, SOCKET clientSocket){
    int iSendResult = send( clientSocket, message.c_str(), message.length()+1, 0 );
    if (iSendResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
    }
    return iSendResult;
}

std::string Network::Recieve(SOCKET clientSocket){
    std::string buf;
    char recvbuf[512];
    int iSendResult;
    int recvbuflen = 512;
    int iResult;

    do {
        std::cout << "STILL IN " <<std::endl;
        iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            // KEEP APPENDING TO RESULT STR AND THEN RETURN  (TEST AND CHECK)
            if (mIResult < recvbuflen){
                recvbuf[mIResult] = '\0';
            }
            buf += recvbuf; // char*
            return buf;
        }
        else if (iResult == 0){
            printf("Client Exited\n");
            break;
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            break;
        }

    } while (iResult > 0);

    return buf;

}

void Network::Cleanup(SOCKET socket){
    int result = shutdown(socket, SD_SEND);
    if (result == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(socket);
        WSACleanup();
        return;
    }
}

void Network::bindSocket(){

    int res;

    printf("3");

#if 0
    res = WSAStartup(MAKEWORD(2,2), &mWsaData);
    if (res != 0) {
        printf("WSAStartup failed with error: %d\n", res);
        return;
    }

    if(mSocketBound != 0){
        return;
    }
#endif 

    struct addrinfo *result = NULL;
    SOCKET ListenSocket = INVALID_SOCKET;
    int iResult;



    // Resolve the server address and port
    printf("PRING PORT %s\n", mPortNumber.c_str());
    iResult = getaddrinfo(NULL, mPortNumber.c_str(), &mHints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return;
    }

    // Create a SOCKET for connecting to server
    mListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (mListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind( mListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(mListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    iResult = listen(mListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(mListenSocket);
        WSACleanup();
        return;
    }
    mSocketBound = 1;
    printf("GOOD BIND ON %d\n", mListenSocket);
}