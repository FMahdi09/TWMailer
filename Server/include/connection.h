#ifndef CONNECTION_H
#define CONNECTION_H

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <iostream>
#include <string>
#include <exception>

class Connection
{
public:
    // ctor
    Connection(int socket);

    // public methods
    void sendMsg(std::string toSend);
    std::string recvMsg();

    // public variables
    const static int headerSize = 4;

private:
    // private variables
    int sock;
};

#endif // CONNECTION_H
