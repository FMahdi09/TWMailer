#ifndef MESSAGEINPUT_H
#define MESSAGEINPUT_H

#include <iostream>
#include <string>
#include <exception>

class MessageInput
{
    public:
        // ctor
        MessageInput();

        // public methods
        std::string inputSend();
        std::string inputList();
        std::string inputRead();
        std::string inputDel();
        std::string inputLogin(std::string& username);

    private:
        std::string username;
};

#endif // MESSAGEINPUT_H
