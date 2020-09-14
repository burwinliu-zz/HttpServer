#include "socketWrapper.h"

//todo buffer management.
//todo Exception handling

// Constructs/Destructors
SocketWrapper::SocketWrapper(int pPortNumber){
    int res;
    mPortNumber = std::to_string(pPortNumber);
    res = WSAStartup(MAKEWORD(2,2), &mWsaData);

    if (res != 0) {
        printf("WSAStartup failed with error: %d\n", res);
        mSetupSuccess = 0;
        return;
    }
    ZeroMemory(&mHints, sizeof(mHints));
    mHints.ai_family = AF_INET;
    mHints.ai_socktype = SOCK_STREAM;
    mHints.ai_protocol = IPPROTO_TCP;
    mHints.ai_flags = AI_PASSIVE;
    mSetupSuccess = 1;
}

SocketWrapper::~SocketWrapper(){
    // cleanup
    // No longer need server socket
    if (mListenSocket != INVALID_SOCKET){
        closesocket(mListenSocket);
    }
    WSACleanup();
}


// Methods and functions
SOCKET SocketWrapper::Accept(sockaddr *addr, int *addrlen){
    SOCKET clientSocket = INVALID_SOCKET;
    int res;

    struct addrinfo *result = NULL;
    SOCKET ListenSocket = INVALID_SOCKET;
    int iResult;



    // Resolve the server address and port
    printf("PRING PORT %s\n", mPortNumber.c_str());
    iResult = getaddrinfo(NULL, mPortNumber.c_str(), &mHints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        return INVALID_SOCKET;
    }

    // Create a SOCKET for connecting to server
    mListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (mListenSocket == INVALID_SOCKET) {
        printf("socket in BindSocket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        return INVALID_SOCKET;
    }

    // Setup the TCP listening socket
    iResult = bind( mListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind in BindSocket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(mListenSocket);
        return INVALID_SOCKET;
    }

    freeaddrinfo(result);

    iResult = listen(mListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen in BindSocket in BindSocket failed with error: %d\n", WSAGetLastError());
        closesocket(mListenSocket);
        return INVALID_SOCKET;
    }
    mSocketBound = 1;

    clientSocket = accept(mListenSocket, addr, addrlen);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "accept in Accept failed with error: " << WSAGetLastError() << std::endl;
        std::cout << "mListenSocket is " << mListenSocket << std::endl;
        closesocket(mListenSocket);
    }
    return clientSocket;
}

int SocketWrapper::Send(std::string message, SOCKET clientSocket){
    printf("SENDING %s WITH %d\n", message.c_str(),message.length()+1);
    int iSendResult = send( clientSocket, message.c_str(), message.length()+1, 0 );
    if (iSendResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(clientSocket);
    }
    return iSendResult;
}

std::string SocketWrapper::Recieve(SOCKET clientSocket){
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
            break;
        }

    } while (iResult > 0);

    return buf;

}

void SocketWrapper::Cleanup(SOCKET socket){
    int result = shutdown(socket, SD_SEND);
    if (result == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(socket);
        return;
    }
}

int SocketWrapper::getSetupSuccess(){
    return mSetupSuccess;
}