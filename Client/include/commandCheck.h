#ifndef COMMANDCHECK_H
#define COMMANDCHECK_H

#include <iostream>
#include <string>
#include <exception>
#include <memory>

#include "messageInput.h"

class CommandCheck
{
    public:
        // ctor
        CommandCheck();

        // public methods
        std::string check(std::string command);

    private:
        // private variables
        std::unique_ptr<MessageInput> messageInput;
};

#endif // COMMANDCHECK_H
