#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>
#include <map>
#include <chrono>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#include "connection.h"
#include "logic.h"
#include "ldapmanager.h"

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
    void handleClient(int clientSocket, std::string ipAddress);
    int checkAccessRights(std::string ipAddress);

    // private variables
    int listeningSocket;
    std::string mailDirectory;
    std::map<std::string, std::mutex> indexLocks;

    std::mutex blacklistMutex;
    std::map<std::string, std::chrono::time_point<std::chrono::system_clock>> blacklist;
};

#endif // SERVER_H
