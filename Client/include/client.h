#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <memory>

#include "connection.h"

class Client
{
public:
    // ctor
    Client(int port, std::string ipAddr);

    // public methods
    void start();

private:
    // private variables
    std::unique_ptr<Connection> connection;
};

#endif // CLIENT_H
