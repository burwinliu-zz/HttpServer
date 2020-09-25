#ifndef HTTPSERVER_NETWORK_H_
#define HTTPSERVER_NETWORK_H_

#include <functional>

#include "socketWrapper.h"


#pragma comment (lib, "Ws2_32.lib")



class Network{
    public:
        // Constructs/Destructors
        Network(int pPortNumber=8000);
        
        ~Network();  

        int SetResponseFunction(std::function<std::string (std::string)>);

        
    private:
        SocketWrapper pWrapper;
        std::function<std::string (std::string)> pResponseFunction;

        
};


#endif