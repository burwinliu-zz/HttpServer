#include "socketWrapper.h"

SocketWrapper::SocketWrapper(){
    int iResult;
    
    iResult = WSAStartup(MAKEWORD(2,2), &mWsaData);
    if (iResult != 0) {
        mSetup = -1;
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

SocketWrapper::SocketWrapper(int socket, addrinfo info, bool bound, bool connected)
    : mSock(socket), mBound(bound), mConnected(connected)
{
    mInfo = new addrinfo(info);
}

void SocketWrapper::bind(int port){
    if (mSetup == 1){
        if(mBound && mConnected)
            printf("Already bound\n");
            return;

        setInfo("null", port);

        addrinfo * result;
        for(result = mInfo; result != NULL; result = mInfo->ai_next)
        {
            if(!mSockCreated)
            {
                openSocket(result);
                if(mSock == -1){
                    continue;
                }
            }
            //Socket sucessfully opened from here
            if( ::bind(mSock, result->ai_addr, result->ai_addrlen) == 0)
            {
                mBound = true;
                return;
            }
        }
        //Couldn't bind, throw
        printf("Can't bind to port\n");
    }
}

void SocketWrapper::connect(std::string addr, int port){
    if (mSetup == 1){
        if(mConnected)
            std::cout << "Already connected" << std::endl;
            return;

        setInfo(addr, htons(port));

        addrinfo * result;
        for(result = mInfo; result != NULL; result = mInfo->ai_next)
        {
            if(!mSockCreated)
            {
                openSocket(result);
                if (mSock == -1){
                    continue;
                }
            }
            //Socket sucessfully opened from here
            if( ::connect(mSock, result->ai_addr, result->ai_addrlen) == 0)
            {
                mConnected = true;
                return;
            }
        }
        //Couldn't connect, throw
        std::cout << "Can't connect to host" << std::endl;
    }
}

void SocketWrapper::listen(){
    if (mSetup == 1){
        if( ::listen(mSock, SOMAXCONN) != 0){
            std::cout << "LISTEND ERROR: " << std::to_string(errno) << std::endl;
        }
    }
}

std::shared_ptr<SocketWrapper> SocketWrapper::accept(){
    if (mSetup == 1){
        union
        {
            sockaddr addr;
            sockaddr_in in;
            sockaddr_in6 in6;
            sockaddr_storage s;
        } address;
        socklen_t addressSize = sizeof (sockaddr_storage);

        int newSock;
        if( (newSock = ::accept(mSock, (sockaddr*)&address.s, &addressSize)) == -1){
            std::cout << "ACCEPT ERROR: " << std::string(strerror(errno)) << std::endl;
            return nullptr;
        }


        addrinfo info;
        memset(&info, 0, sizeof info);
        if(address.s.ss_family == AF_INET)
        {
            info.ai_family = AF_INET;
            info.ai_addr = new sockaddr(address.addr);
        }
        else
        {
            info.ai_family = AF_INET6;
            info.ai_addr = new sockaddr(address.addr);
        }

        return std::shared_ptr<SocketWrapper>(new SocketWrapper(newSock, info, true, false));
    }
    return nullptr;
}

void SocketWrapper::send(const char *data, unsigned int length, int flags){
    if (mSetup == 1){
        const char * buff = data;
        int status = 0;
        int total_sent = 0;
        int left_to_send = length;
        while(total_sent < length)
        {
            status = ::send(mSock, buff + total_sent, left_to_send, flags);
            if(status == -1)
            {
                std::cout << "SEND ERROR: " << std::string(strerror(errno)) << std::endl;
                return;
            }
            else
            {
                total_sent += status;
                left_to_send -= status;
            }
        }
    }
}

int SocketWrapper::receive(char* msg, int len, int flags){
    if (mSetup == 1){
        int status;
        if( (status = ::recv(mSock, msg, len, flags)) == -1){
            std::cout << "RECEIVE ERROR: " << std::string(strerror(errno)) << std::endl;
            return -1;
        }
        else if(status == 0)
            return 0;

        return 1;
    }
    return -1;
}

void SocketWrapper::close(){
    if (mSetup == 1){
        if( ::closesocket(mSock) == -1){
            std::cout << "CLOSE ERROR: " << std::string(strerror(errno)) << std::endl;
            return;
        }
        else
            mClosed = true;
    }
}

void SocketWrapper::setInfo(std::string address, int port){
    if (mSetup == 1){
        const char *charAddress;
        int status;

        if(address == "null")
            charAddress = NULL;
        else
            charAddress = address.c_str();

        addrinfo hints = *mInfo;
        
        if( (status = getaddrinfo(charAddress, std::to_string(port).c_str(), &hints, &mInfo)) != 0)
        {
            delete charAddress;
            std::cout << "getaddrinfo returned non-zero : " << std::to_string(status) << std::endl;
        }
        delete charAddress;
    }
}

void SocketWrapper::openSocket(addrinfo *info){
    if (mSetup == 1){
        mSock = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
        if(mSock == -1)
        {
            std::cout << "openSocket threw error with code: " << std::string(strerror(errno)) << std::endl;
        }
    }
}