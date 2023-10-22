#ifndef COMMANDCHECK_H
#define COMMANDCHECK_H

#include <iostream>
#include <string>
#include <exception>

#include "connection.h"

class CommandCheck
{
    public:
        // ctor
        CommandCheck();

        // public methods
        void check(std::string command);

    private:

};

#endif // COMMANDCHECK_H
