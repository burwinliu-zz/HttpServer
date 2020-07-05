#include "socketWrapper.h"
#include "network.h"

using SocketPointer = std::shared_ptr<SocketWrapper>;

int __cdecl main(void){
    SOCKET client;

    printf("0");
    Network net = Network();

    printf("1");
    client = net.Accept(NULL, NULL);
    std::cout << net.Recieve(client);
    net.Send("TESTING", client);
    net.Cleanup(client);


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

    // return 0;
}