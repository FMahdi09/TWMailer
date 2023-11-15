#include "connection.h"

// ctor
Connection::Connection(int socket)
{
    sock = socket;

    // init cipher
    cipher = std::make_unique<Cipher>();

    if(!cipher->readKeyFromFile())
    {
        cipher->generateKeyPair();
    }

    AESready = false;
}

// public functions

//send handed over message to client
void Connection::sendMsg(std::string toSend)
{
    if(AESready)
    {
        toSend = cipher->AESencrypt(toSend);
    }

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

//receive message from client
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
            throw std::runtime_error("client shutdown unexpectedly");
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
            throw std::runtime_error("client shutdown unexpectedly");
        default:
            bytesRecv += temp;
        }
    }

    msgBuffer[msgSize] = '\0';

    std::string toReturn = std::string(msgBuffer, msgSize);

    if(AESready)
    {
        toReturn = cipher->AESdecrypt(toReturn);
    }

    return toReturn;
}

// crypto

void Connection::sendPublicKey()
{
    // get public key
    std::string publicKey = cipher->getPublicKey();

    // send public key
    sendMsg(publicKey);
}

void Connection::recvPublicKey()
{
    // recv public key
    std::string publicKey = recvMsg();

    // set public key
    cipher->readOtherPublicKey(publicKey);
}

void Connection::AESinit()
{
    // create passphrase and salt
    std::string passphrase = cipher->generateRandomString(50);
    std::string salt = cipher->generateRandomString(20);

    // encrypt with RSA
    std::string encryptedPassphrase = cipher->RSAencrypt(passphrase);
    std::string encryptedSalt = cipher->RSAencrypt(salt);

    // AESinit
    cipher->AESinit(passphrase, salt);

    // send encrypted msgs to client
    sendMsg(encryptedPassphrase);
    sendMsg(encryptedSalt);

    AESready = true;
}















