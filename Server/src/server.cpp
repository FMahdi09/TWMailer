#include "server.h"

// ctor
Server::Server(int port, std::string spoolDir)
{
    abortRequested = false;

    // create listeningsocket
    if((listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw std::runtime_error("unable to create listening socket");

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
    struct sockaddr_in clientAddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    // main loop
    while(!abortRequested)
    {
        // accept incoming connection
        if((clientSocket = accept(listeningSocket, (sockaddr*)&clientAddr, &addrlen)) == -1)
        {
            if(abortRequested)
                break; // exit loop

            throw std::runtime_error("failed to accept new connection");
        }

        // print information about the client
        std::cout << "Client connected from "
                  << inet_ntoa(clientAddr.sin_addr) << ":"
                  << ntohs(clientAddr.sin_port) << "\n";


        // create child process to handle connection
        pid_t pid = fork();
        lastChild = pid;

        switch(pid)
        {
        case -1: // error
            throw std::runtime_error("failed to create child process");
        case 0: // child
            std::cout << "Child started\n";
            listeningSocket = -1;
            handleClient();
            return;
        }
    }

    // cleanup:

    // close listening socket
    if(listeningSocket != -1)
    {
        shutdown(listeningSocket, SHUT_RDWR);
        close(listeningSocket);
    }

    // send SIGINT to "all" children
    kill(-lastChild, SIGINT);
}

void Server::abort()
{
    if(listeningSocket != -1)
    {
        shutdown(listeningSocket, SHUT_RDWR);
        close(listeningSocket);
        listeningSocket = -1;
    }

    if(clientSocket != -1)
    {
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
        clientSocket = -1;
    }

    abortRequested = true;
}

// private methods

void Server::handleClient()
{
    std::string request, response;
    auto connection = std::make_unique<Connection>(clientSocket);

    while(!abortRequested)
    {
        try
        {
            // get request
            request = connection->recvMsg();

            if(request == "QUIT" || request == "")
                break; // exit loop

            // get response
            response = "recv: " + request + "\n";

            // send response
            connection->sendMsg(response);
        }
        catch(std::runtime_error const& ex)
        {
            if(!abortRequested)
                std::cerr << ex.what() << std::endl;
        }
    }

    std::cout << "Child stopped\n";

    if(clientSocket != -1)
    {
        std::cout << "close listening not aborted\n";
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
    }
}
