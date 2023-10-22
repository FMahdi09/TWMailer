#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>

class Logic
{
public:
    //ctor
    Logic(std::string spoolDir);

    // public methods
    std::string getResponse(std::string request);

private:
    // private methods
    std::string send(std::stringstream& request);
    std::string list(std::stringstream& request);
    std::string read(std::stringstream& request);
    std::string del(std::stringstream& request);

    void createNewUser(std::string username);
    void createNewMessage(std::string username, std::string subject, std::string msg);
    std::string getSubject(std::string filepath);
    std::string getBody(std::string filepath);

    // private variables
    std::string headDir;
};

#endif // LOGIC_H
