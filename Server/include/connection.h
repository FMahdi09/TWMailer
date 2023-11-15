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
#include <memory>

#include "cipher.h"

class Connection
{
public:
    // ctor
    Connection(int socket);

    // public methods
    void sendMsg(std::string toSend);
    std::string recvMsg();

    // crypto
    void sendPublicKey();
    void recvPublicKey();
    void AESinit();

    // public variables
    const static int headerSize = 4;

private:
    // private variables
    bool AESready;
    int sock;
    std::unique_ptr<Cipher> cipher;
};

#endif // CONNECTION_H
