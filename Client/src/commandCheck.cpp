#include "commandCheck.h"

// ctor
CommandCheck::CommandCheck()
{
    messageInput = std::make_unique<MessageInput>();
}

// public functions

std::string CommandCheck::check(std::string command)
{
    if(command == "SEND")
    {
        return messageInput->inputSend();
    }
    else if (command == "LIST")
    {
        std::cout << "2 " << command << std::endl;
    }
    else if (command == "READ")
    {
        std::cout << "3 " << command << std::endl;
    }
    else if (command == "DEL")
    {
        std::cout << "4 " << command << std::endl;
    }
    else if (command == "QUIT")
    {
        std::cout << "5 " << command << std::endl;
        return command;
    }
    else
    {
        throw std::invalid_argument("no known argument");
    }
    return "?";
}