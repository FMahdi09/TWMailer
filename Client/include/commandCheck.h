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
        std::string checkMainloop(std::string command);
        std::string checkLogin(std::string command, std::string& username);

    private:
        // private variables
        std::unique_ptr<MessageInput> messageInput;
};

#endif // COMMANDCHECK_H
