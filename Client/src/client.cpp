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
            toSend = commandCheck->check(command);

            // send request
            connection->sendMsg(toSend);

            if(toSend == "QUIT")
                return;

            // print response
            std::cout << connection->recvMsg();
        }
        catch (std::invalid_argument const& ex)
        {
            std::cerr << ex.what() << "\n";
        }
    }
}
