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
#include <sys/wait.h>

#include "connection.h"
#include "logic.h"

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
    std::shared_ptr<Logic> logic;

    int listeningSocket;
    int clientSocket;
    pid_t lastChild;
    bool abortRequested;
};

#endif // SERVER_H
