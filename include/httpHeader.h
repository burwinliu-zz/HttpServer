#ifndef HTTPSERVER_HTTPHEADER_H_
#define HTTPSERVER_HTTPHEADER_H_


#pragma comment (lib, "Ws2_32.lib")

#include <string>

class HttpHeader{
    public:
        // Constructs/Destructors
        HttpHeader(int messageLen, std::string contentType);
        HttpHeader(std::string status);

        std::string getHeader();

        
    private:
        std::string mHeader;

        
};

#endif