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
        void inputList();
        void inputRead();
        void inputDel();

    private:

};

#endif // MESSAGEINPUT_H
