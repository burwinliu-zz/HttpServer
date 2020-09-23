#include "server.h"

using SocketPointer = std::shared_ptr<SocketWrapper>;

int __cdecl main(void){
    SOCKET client;

    HttpServer server = HttpServer(8080);

    server.StartServer();

    // while(1){
    //     client = sock.Accept(NULL, NULL);
    //     std::cout << sock.Receive(client);
    //     sock.Send("TESTING\n\0", client);
    //     sock.Cleanup(client); 
    // }


    // SocketPointer sock(new SocketWrapper);
    // SocketPointer client;
    // sock->bind(1170);
    // sock->listen();
    // client = sock->accept();

    // //Welcoming the new user.
    // client->send("Welcome !\n\f", 15, 0);
    // //Closing the listening soket, we want nobody else.
    // sock->close();

    // char data[512];
    // memset(&data, 0, 512);
    // while( client->receive(data, sizeof data, 0) )
    // {
    //     client->send(data, sizeof data, 0);
    //     memset(&data, 0, 512);
    // }

    // client->close();

    return 0;
}