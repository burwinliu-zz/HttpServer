#include "server.h"

int __cdecl main(void){
    Network network = Network();
    
    int iResult;
    SOCKET ListenSocket = network.bindSocket();
    SOCKET ClientSocket = INVALID_SOCKET;
    char *helloPage = "HTTP/1.1 200 OK\nContent-Type: text/HTML\nContent-Length: 12\n\nHello world!";
    char *errorPage = "HTTP/1.1 404 BAD REQUEST\nContent-Type: text/plain\nContent-Length: 12\n\n404 BAD REQUEST";

    int iSendResult;
    char recvbuf[512];
    int recvbuflen = 512;
    
    printf("THE BIND WENT THRU %d\n", ListenSocket);


    while(1){
        // Accept a client socket
        SOCKET clientSocket = accept(ListenSocket, NULL, NULL);
        printf("DIDN'T HIT %d\n", clientSocket);
        if (clientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
        }
        printf("DIDN'T HIT %d\n", clientSocket);

        // Receive until the peer shuts down the connection
        do {

            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                printf("Bytes received: %d\n", iResult);
                printf("%s\n", recvbuf);
                printf("PARSE %d\n", strncmp(recvbuf, "GET / HTTP", 10));
                if(strncmp(recvbuf, "GET / HTTP", 10) == 0){
                    iSendResult = send( ClientSocket, helloPage, iResult, 0 );
                }
                else{
                    iSendResult = send( ClientSocket, errorPage, iResult, 0 );
                }
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return 1;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (iResult == 0){
                printf("Client Exited\n");
                break;
            }
            else {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                break;
            }

        } while (iResult > 0);

        // shutdown the connection since we're done
        iResult = shutdown(ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    }

    // cleanup
    // No longer need server socket
    
    closesocket(ClientSocket);
    WSACleanup();
    free(helloPage); /* Don't forget to call free() for page that has been allocated memory */
    return 0;
}