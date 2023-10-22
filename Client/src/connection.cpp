#include "connection.h"

// ctor
Connection::Connection(int port, std::string ipAddr)
{
    // create socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw std::runtime_error("unable to create socket");

    // create address hint
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_aton(ipAddr.c_str(), &address.sin_addr);

    // create connection
    if(connect(sock, (sockaddr*)&address, sizeof(address)) == -1)
        throw std::runtime_error("server not available");
}

// destructor
Connection::~Connection()
{
    // free resources
    if(sock != -1)
    {
        shutdown(sock, SHUT_RDWR);
        close(sock);
    }
}

// public functions

//send handed over message to server
void Connection::sendMsg(std::string toSend)
{
    int temp, bytesSent = 0;
    int msgSize = toSend.size();

    // set header
    uint8_t headerBuffer[4];
    memcpy(&headerBuffer[0], &msgSize, 4);

    // send header
    while(bytesSent < headerSize)
    {
        temp = send(sock, &headerBuffer[bytesSent], headerSize - bytesSent, 0);

        switch(temp)
        {
        case -1: // send error
            throw std::runtime_error("error when sending message");
        default:
            bytesSent += temp;
        }
    }

    bytesSent = 0;

    // send body
    while(bytesSent < msgSize)
    {
        temp = send(sock, &toSend[bytesSent], msgSize - bytesSent, 0);

        switch(temp)
        {
        case -1: // send error
            throw std::runtime_error("error when sending message");
        default:
            bytesSent += temp;
        }
    }
}

//receive and return (answer)message from server
std::string Connection::recvMsg()
{
    int temp, bytesRecv = 0;
    uint8_t headerBuffer[4];

    // recv header
    while(bytesRecv < headerSize)
    {
        temp = recv(sock, &headerBuffer[bytesRecv], headerSize - bytesRecv, 0);

        switch(temp)
        {
        case -1: // recv error
            throw std::runtime_error("error when sending message");
        case 0: // server shutdown
            throw std::runtime_error("server shutdown unexpectedly");
        default:
            bytesRecv += temp;
        }
    }

    // get size from header
    int msgSize;
    memcpy(&msgSize, &headerBuffer[0], 4);

    // create buffer for msg
    char msgBuffer[msgSize + 1];

    bytesRecv = 0;

    // recv body
    while(bytesRecv < msgSize)
    {
        temp = recv(sock, &msgBuffer[bytesRecv], msgSize - bytesRecv, 0);

        switch(temp)
        {
        case -1: // recv error
            throw std::runtime_error("error when sending message");
        case 0: // server shutdown
            throw std::runtime_error("server shutdown unexpectedly");
        default:
            bytesRecv += temp;
        }
    }

    msgBuffer[msgSize] = '\0';

    return msgBuffer;
}














