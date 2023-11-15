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
    bool isLogin = false;

    // crypto handshake

    // send public key to client
    connection->sendPublicKey();

    // recv public key from client
    connection->recvPublicKey();

    // AES init
    connection->AESinit();

    while(true)
    {
        //Possible Commands message
        std::cout << "\nPossible Commands: \nLOGIN\nQUIT\n" << std::endl;

        // get request
        getline(std::cin, command);

        try
        {
            std::string username;
            username = "test";

            //function call to check entered command
            //return input to send for entered command
            toSend = commandCheck->checkLogin(command, username);                                    

            // send request with message or text
            connection->sendMsg(toSend);

            if(toSend == "QUIT")
            {
                return;
            }

            // print response
            std::string response = connection->recvMsg();
            std::cout << response;
            if(response == "OK\n")
            {
                isLogin = true;
                break;
            }
        }
        catch(std::exception const& ex)
        {
            //catch for invalid command entered
            //catch if unexpected error orruced during message input
            std::cerr << ex.what() << "\n";
        }
    }

    if(isLogin)
    {
        mainloop();
    }
}

void Client::mainloop()
{
    std::string toSend;
    std::string command;

    // main loop
    while(true)
    {
        //Possible Commands message
        std::cout << "\nPossible Commands: \nSEND\nLIST\nREAD\nDEL\nQUIT\n" << std::endl;

        // get request
        getline(std::cin, command);

        //if entered command is not valid/existent, error message will be printed
        try
        {
            //function call to check entered command
            //return input to send for entered command
            toSend = commandCheck->checkMainloop(command);

            // send request with message or text
            connection->sendMsg(toSend);

            if(toSend == "QUIT")
            {
                return;
            }

            // print response
            std::cout << connection->recvMsg();
        }
        catch(std::exception const& ex)
        {
            //catch for invalid command entered
            //catch if unexpected error orruced during message input
            std::cerr << ex.what() << "\n";
        }
    }
}
