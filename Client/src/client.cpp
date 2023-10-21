#include "client.h"

// ctor
Client::Client(int port, std::string ipAddr)
{
    // init connection
    connection = std::make_unique<Connection>(port, ipAddr);
}

// public functions

void Client::start()
{
    std::string toSend;

    // main loop
    while(true)
    {
        // get request
        getline(std::cin, toSend);

        // send request
        connection->sendMsg(toSend);

        if(toSend == "QUIT")
            return;

        // print response
        std::cout << connection->recvMsg();
    }
}
