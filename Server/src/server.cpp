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

    std::cout << "started listening on port: " << port << std::endl;
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

            std::string clientAddress = inet_ntoa(clientAddr.sin_addr);

            // print information about the client
            std::cout << "Client connected from "
                      << clientAddress << ":"
                      << ntohs(clientAddr.sin_port) << "\n";

            // start new thread to handle client
            std::thread clientHandler = std::thread(&Server::handleClient, this, clientSocket, clientAddress);
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

void Server::handleClient(int clientSocket, std::string ipAddress)
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

    // check if client is blacklisted
    int secondsRemaining;

    if((secondsRemaining = checkAccessRights(ipAddress)) != 0)
    {
        connection->sendMsg("You are blacklisted!\nTry again in " + std::to_string(secondsRemaining) + " seconds.\n");
        return;
    }

    connection->sendMsg("Welcome to the Server\n");


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
        catch(std::invalid_argument const& ex)
        {
            // blacklist user
            blacklistMutex.lock();

            blacklist[ipAddress] = std::chrono::system_clock::now() + std::chrono::minutes(1);

            blacklistMutex.unlock();

            connection->sendMsg(ex.what());
            break;
        }
    }

    // cleanup:

    // close client socket
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}

int Server::checkAccessRights(std::string ipAddress)
{
    blacklistMutex.lock();

    if(blacklist.find(ipAddress) != blacklist.end())
    {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>
            (blacklist[ipAddress] - std::chrono::system_clock::now());

        if(seconds.count() <= 0)
        {
            blacklist.erase(ipAddress);
            blacklistMutex.unlock();
            return 0;
        }

        blacklistMutex.unlock();
        return seconds.count();
    }

    blacklistMutex.unlock();
    return 0;
}













