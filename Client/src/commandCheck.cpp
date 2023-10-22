#include "commandCheck.h"

// ctor
CommandCheck::CommandCheck()
{

}

// public functions

void CommandCheck::check(std::string command)
{
    if(command == "SEND")
    {
        std::cout << "1 " << command << std::endl;
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
    }
    else
    {
        throw "no known argument";
    }
}