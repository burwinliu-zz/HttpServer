#include "network.h"


// Constructs/Destructors
Network::Network(int pPortNumber){
    mPortNumber = std::to_string(pPortNumber);
}

Network::~Network(){
    // cleanup
    // No longer need server socket
    closesocket(mListenSocket);
    WSACleanup();
}


// Methods and functions
SOCKET Network::Accept(sockaddr *addr, int *addrlen){
    std::cout <<mListenSocket << " SOCKET NUM 1" <<std::endl;
    SOCKET clientSocket = accept(mListenSocket, NULL, NULL);
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

        iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            // KEEP APPENDING TO RESULT STR AND THEN RETURN  (TEST AND CHECK)
            if (mIResult < recvbuflen){
                recvbuf[mIResult] = '\0';
            }
            buf += recvbuf; // char*
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

SOCKET Network::bindSocket(){
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;


    struct addrinfo *result = NULL;
    struct addrinfo hints;


    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return INVALID_SOCKET;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, mPortNumber.c_str(), &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }
    printf("GOOD BIND ON %d\n", ListenSocket);
    return ListenSocket;
}