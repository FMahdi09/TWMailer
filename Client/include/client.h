#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <memory>

#include "connection.h"
#include "commandCheck.h"
#include "cipher.h"

class Client
{
public:
    // ctor
    Client(int port, std::string ipAddr);

    // public methods
    void start();
    void mainloop();

private:
    // private variables
    std::unique_ptr<Connection> connection;
    std::unique_ptr<CommandCheck> commandCheck;
    std::unique_ptr<Cipher> cipher;
};

#endif // CLIENT_H
