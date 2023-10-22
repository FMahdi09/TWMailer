#include "commandCheck.h"

// ctor
CommandCheck::CommandCheck()
{
    messageInput = std::make_unique<MessageInput>();
}

// public functions

std::string CommandCheck::check(std::string command)
{
    //call correct function to command given
    if(command == "SEND")
    {
        return messageInput->inputSend();
    }
    else if (command == "LIST")
    {
        return messageInput->inputList();
    }
    else if (command == "READ")
    {
        return messageInput->inputRead();
    }
    else if (command == "DEL")
    {
        return messageInput->inputDel();
    }
    else if (command == "QUIT")
    {
        return command;
    }
    else
    {
        //if no valid command is entered
        throw std::invalid_argument("no known argument");
    }
    //if unknown error occure
    throw std::logic_error("unknown error");
}