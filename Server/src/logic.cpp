#include "logic.h"

namespace fs = std::filesystem;

Logic::Logic(std::string spoolDir)
{
    // create spoolDir if not exists
    if(!fs::exists(spoolDir))
        fs::create_directory(spoolDir);

    headDir = spoolDir;
}

// public methods

std::string Logic::getResponse(std::string request)
{
    std::stringstream requestStream;
    requestStream << request;

    std::string method;
    getline(requestStream, method);

    if(method == "SEND")
        return send(requestStream);

    if(method == "LIST")
        return send(requestStream);

    if(method == "READ")
        return send(requestStream);

    if(method == "DEL")
        return send(requestStream);

    if(method == "QUIT")
        return "";

    return "ERR: unknown method\n";
}

// private methods

std::string Logic::send(std::stringstream& request)
{
    std::string curLine, sender, reciever, subject, msg;

    // read values from request
    if(!std::getline(request, sender) ||
       !std::getline(request, reciever) ||
       !std::getline(request, subject) ||
       !std::getline(request, curLine))
        return "ERR\n";

    // read msg body
    while(curLine != ".")
    {
        msg.append(curLine);
        msg.append("\n");

        if(!std::getline(request, curLine))
            return "ERR\n";
    }

    // create new user dir if not exists
    if(!fs::exists(headDir + "/" + reciever))
        createNewUser(reciever);

    // create msg
    createNewMessage(reciever, subject, msg);

    return "OK\n";
}

void Logic::createNewUser(std::string username)
{
    fs::create_directory(headDir + "/" + username);

    std::ofstream indexFile("index");
    indexFile << "1";
    indexFile.close();
}

void Logic::createNewMessage(std::string username, std::string subject, std::string msg)
{
    // get index for new msg
    std::string userpath = headDir + "/" + username;

    std::ifstream inIndexFile(userpath + "/index");
    int index;
    inIndexFile >> index;
    inIndexFile.close();

    // increment index
    std::ofstream outIndexFile(userpath + "/index");
    outIndexFile << index + 1;
    outIndexFile.close();

    // create file
    std::ofstream newMsgFile(userpath + "/" + std::to_string(index) + "-" + subject);

    // write message to file
    newMsgFile << msg;
    newMsgFile.close();
}