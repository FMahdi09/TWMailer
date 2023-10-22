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
        getline(std::cin, command);

        //if entered command is not valid/existent, error message will be printed
        try
        {
            //function call to check entered command
            //return input to send for entered command
            toSend = commandCheck->check(command);

            // send request with message or text
            connection->sendMsg(toSend);

            if(toSend == "QUIT")
                return;

            // print response
            std::cout << connection->recvMsg();
        }
        catch (std::invalid_argument const& ex)
        {
            //catch for invalid command entered
            std::cerr << ex.what() << "\n";
        }
        catch(std::exception const& ex)
        {
            //catch if unexpected error orruced during message input
            std::cerr << ex.what() << "\n";
        }
    }
}
