#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <set>
#include <map>
#include <mutex>

#include "ldapmanager.h"

class Logic
{
public:
    //ctor
    Logic(std::string spoolDir, std::map<std::string, std::mutex> &indexLocks);

    // public methods
    std::string getResponse(std::string request);

private:
    // private methods
    std::string login(std::stringstream& request);
    std::string send(std::stringstream& request);
    std::string list(std::stringstream& request);
    std::string read(std::stringstream& request);
    std::string del(std::stringstream& request);

    void createNewUser(std::string username);
    void createNewMessage(std::string username, std::string subject, std::string msg);
    std::string getSubject(std::string filepath);
    std::string getBody(std::string filepath);

    // private variables
    int loginTries;
    std::string curUsername;
    std::string headDir;
    std::map<std::string, std::mutex>* indexLocks;
    std::unique_ptr<LDAPManager> ldapManager;
};

#endif // LOGIC_H
