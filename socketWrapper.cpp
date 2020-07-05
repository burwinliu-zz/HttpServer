#include "socketWrapper.h"

SocketWrapper::SocketWrapper(){
    int iResult;
    
    iResult = WSAStartup(MAKEWORD(2,2), &mWsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return;
    }

    mInfo = new addrinfo;
    memset(mInfo, 0, sizeof *mInfo);
    mInfo->ai_family = AF_UNSPEC;
    mInfo->ai_socktype = SOCK_STREAM;
}

SocketWrapper::~SocketWrapper(){
    freeaddrinfo(mInfo);
    WSACleanup();
}

void SocketWrapper::bind(int port){}
void SocketWrapper::connect(std::string addr, int port){}
void SocketWrapper::listen(){}
void SocketWrapper::accept(){}
void SocketWrapper::send(const char *data, unsigned int length, int flags){}
bool SocketWrapper::receive(char* msg, int len, int flags){}
void SocketWrapper::close(){}

void SocketWrapper::setInfo(int port){}
void SocketWrapper::openSocket(addrinfo *info){}