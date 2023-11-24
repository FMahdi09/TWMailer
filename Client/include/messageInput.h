#ifndef MESSAGEINPUT_H
#define MESSAGEINPUT_H

#include <iostream>
#include <string>
#include <exception>

#include <unistd.h>
#include <termios.h>

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
        // private methods
        std::string readPassword();
        int getch();
};

#endif // MESSAGEINPUT_H
