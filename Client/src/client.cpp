#include "client.h"

// ctor
Client::Client(int port, std::string ipAddr)
{
    // init connection
    connection = std::make_unique<Connection>(port, ipAddr);
    commandCheck = std::make_unique<CommandCheck>();
}

// public functions

void Client::start()
{
    std::string toSend;
    std::string command;

    // main loop
    while(true)
    {
        // get request
        //getline(std::cin, toSend);
        getline(std::cin, command);

        try
        {
            commandCheck->check(command);

        }
        catch (const char* ex)
        {
            std::cerr << ex << "\n";
            continue;
        }
        toSend = command;

        // send request
        connection->sendMsg(toSend);

        if(toSend == "QUIT")
            return;

        // print response
        std::cout << connection->recvMsg();
    }
}
