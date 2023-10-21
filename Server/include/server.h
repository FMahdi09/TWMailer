#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <thread>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "connection.h"

class Server
{
public:
    // ctor
    Server(int port, std::string spoolDir);

    // public methods
    void start();
    void abort();

private:
    // private methods
    void handleClient();

    // private variables
    int listeningSocket;
    int clientSocket;
    pid_t lastChild;
    bool abortRequested;
};

#endif // SERVER_H
