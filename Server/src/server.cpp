#include "server.h"

// ctor
Server::Server(int port, std::string spoolDir)
{
    mailDirectory = spoolDir;

    // check if the directory already exists
    if(std::filesystem::exists(mailDirectory))
    {
        // create a lock for each index file
        for(auto const& dirEntry : std::filesystem::directory_iterator(mailDirectory))
        {
            indexLocks[dirEntry.path().filename()];
        }
    }
    else
    {
        // create spoolDir if it does not exist
        std::filesystem::create_directory(mailDirectory);
    }

    // create listeningsocket
    if((listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw std::runtime_error("unable to create listening socket");

    // set socket options
    int reuseValue = 1;

    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &reuseValue, sizeof(reuseValue)) == -1)
        throw std::runtime_error("unable to set socket options: reuseAddr");

    if (setsockopt(listeningSocket,SOL_SOCKET,SO_REUSEPORT, &reuseValue, sizeof(reuseValue)) == -1)
        throw std::runtime_error("unable to set socket option: reusePort");


    // bind socket to ip address and port
    struct sockaddr_in listeningHint;
    listeningHint.sin_family = AF_INET;
    listeningHint.sin_port = htons(port);
    listeningHint.sin_addr.s_addr = INADDR_ANY;

    if((bind(listeningSocket, (sockaddr*)&listeningHint, sizeof(listeningHint))) == -1)
        throw std::runtime_error("failed to bind listening socket");

    // start listening for connections
    if((listen(listeningSocket, SOMAXCONN)) == -1)
        throw std::runtime_error("failed to listen on listening socket");
}

// public methods

void Server::start()
{
    // hint for client information
    struct sockaddr_in clientAddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int clientSocket;

    // main loop
    bool listening = true;

    while(listening)
    {
        try
        {
            // accept incoming connection
            if((clientSocket = accept(listeningSocket, (sockaddr*)&clientAddr, &addrlen)) == -1)            
                throw std::runtime_error("failed to accept new connection");


            // print information about the client
            std::cout << "Client connected from "
                      << inet_ntoa(clientAddr.sin_addr) << ":"
                      << ntohs(clientAddr.sin_port) << "\n";

            // start new thread to handle client
            std::thread clientHandler = std::thread(&Server::handleClient, this, clientSocket);
            clientHandler.detach();
        }
        catch(std::runtime_error const& ex)
        {
            std::cerr << ex.what() << std::endl;
            listening = false;
        }
    }

    // cleanup:

    // close listening socket
    shutdown(listeningSocket, SHUT_RDWR);
    close(listeningSocket);
}

// private methods

void Server::handleClient(int clientSocket)
{
    std::string request, response;
    auto connection = std::make_unique<Connection>(clientSocket);
    auto logic = std::make_unique<Logic>(mailDirectory, indexLocks);

    // crypto handshake

    // send public key to client
    connection->sendPublicKey();

    // recv public key from client
    connection->recvPublicKey();

    // generate and send symmetrical key to client
    connection->AESinit();

    // main loop
    while(true)
    {
        try
        {
            // get request
            request = connection->recvMsg();

            if(request == "QUIT")
                break; // exit loop

            // get response
            response = logic->getResponse(request);

            // send response
            connection->sendMsg(response);
        }
        catch(std::runtime_error const& ex)
        {            
            std::cerr << ex.what() << std::endl;
            break;
        }
    }

    // cleanup:

    // close client socket
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}
