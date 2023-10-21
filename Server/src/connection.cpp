#include "connection.h"

// ctor
Connection::Connection(int socket)
{
    sock = socket;
}

// public functions

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
        case 0: // client shutdown
            return "";
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
        case 0: // client shutdown
            return "";
        default:
            bytesRecv += temp;
        }
    }

    msgBuffer[msgSize] = '\0';

    return msgBuffer;
}
