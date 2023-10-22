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

    private:

};

#endif // MESSAGEINPUT_H
