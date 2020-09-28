#ifndef HTTPSERVER_SERVER_H_
#define HTTPSERVER_SERVER_H_

#include <string>
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <sstream> 
#include <map>

#include <Windows.h>

#include "socketWrapper.h"
#include "httpHeader.h"


#pragma comment (lib, "Ws2_32.lib")

class HttpServer{
    public:
        // Constructs/Destructors
        HttpServer(int pPortNumber=8080);
        ~HttpServer();  


        // Methods and functions
        int SetResource(std::string pPathName, std::string pRequestType, std::string pResourcePath);
        void StartServer();


        //Structs
        struct PageInfo{
            std::string requestType;
            std::string requestPath;
        };

        struct RequestInfo{
            std::string requestType;
            std::string requestPath;
            std::string requestProtocol;
        };

    private:
        // Class variables
        // For Socket Setup
        SocketWrapper pNetworkHelper;
        int pPortNumber;

        //For serving resources to paths
        std::map<std::string, std::map<std::string, std::string>> mMapResources;


        //Helpers
        std::string constructHttpHeader();
        std::string fileToResponse(std::string pFilePath);
        std::string fileToString(std::string pFilePath);
        struct RequestInfo parseHeader(std::string pResponse);

        std::string HttpServer::constructHttpHeader(std::string pStatus, int pStatusCode, std::string pContentType);

        enum STATUS
        {
            Continue = 100,
            SwitchingProtocol = 101,
            Processing = 102,
            EarlyHints = 103,

            Ok = 200,
            Created = 201,
            Accepted = 202,
            NonAuthoritativeInformation = 203,
            NoContent = 204,
            ResetContent = 205,
            PartialContent = 206,
            MultiStatus = 207,
            AlreadyReported = 208,
            ImUsed = 226,

            MultipleChoice = 300,
            MovedPermanently = 301,
            Found = 302,
            SeeOther = 303,
            NotModified = 304,
            UseProxy = 305,
            TemporaryRedirect = 307,
            PermanentRedirect = 308,

            BadRequest = 400,
            Unauthorized = 401,
            PaymentRequired = 402,
            Forbidden = 403,
            NotFound = 404,
            MethodNotAllowed = 405,
            NotAcceptable = 406,
            ProxyAuthenticationRequired = 407,
            RequestTimeout = 408,
            Conflict = 409,
            Gone = 410,
            LengthRequired = 411,
            PreconditionFailed = 412,
            PayloadTooLarge = 413,
            UriTooLong = 414,
            UnsupportedMediaType = 415,
            RangeNotSatisfiable = 416,
            ExpectationFailed = 417,
            ImaTeapot = 418,
            MisdirectedRequest = 421,
            UnprocessableEntity = 422,
            Locked = 423,
            FailedDependency = 424,
            TooEarly = 425,
            UpgradeRequired = 426,
            PreconditionRequired = 428,
            TooManyRequests = 429,
            RequestHeaderFieldsTooLarge = 431,
            UnavailableForLegalReasons = 451,

            InternalServerError = 500,
            NotImplemented = 501,
            BadGateway = 502,
            ServiceUnavailable = 503,
            GatewayTimeout = 504,
            HttpVersionNotSupported = 505,
            VariantAlsoNegotiates = 506,
            InsufficientStorage = 507,
            LoopDetected = 508,
            NotExtended = 510,
            NetworkAuthenticationRequired = 511
        };
};


#endif