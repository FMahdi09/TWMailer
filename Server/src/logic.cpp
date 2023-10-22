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
        return list(requestStream);

    if(method == "READ")
        return read(requestStream);

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

    // check if values are valid
    if(sender == "" ||
       reciever == "" ||
       subject == "")
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

std::string Logic::list(std::stringstream& request)
{
    std::string username, userpath, filename, msg;
    int fileCount = 0;

    // read values from request
    if(!std::getline(request, username))
        return "ERR\n";

    // check if values are valid
    if(username == "")
        return "ERR\n";

    userpath = headDir + "/" + username;

    // return "0\n" if user unknown
    if(!fs::exists(userpath))
        return "0\n";

    // iterate over user dir and add all messages
    for(auto const& dir_entry : fs::directory_iterator(userpath))
    {
        if((filename = fs::path(dir_entry).filename()) != "index")
        {
            msg.append(filename + "-" + getSubject(userpath + "/" + filename) + "\n");
            ++fileCount;
        }
    }

    return std::to_string(fileCount) + "\n" + msg;
}

std::string Logic::read(std::stringstream& request)
{
    std::string username, msgNumber;

    // read values from request
    if(!std::getline(request, username) ||
       !std::getline(request, msgNumber))
        return "ERR\n";

    // check if values are valid
    if(username == "" ||
       msgNumber == "")
        return "ERR\n";

    return getBody(headDir + "/" + username + "/" + msgNumber);
}

void Logic::createNewUser(std::string username)
{
    fs::create_directory(headDir + "/" + username);

    std::ofstream indexFile(headDir + "/" + username + "/index");
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
    std::ofstream newMsgFile(userpath + "/" + std::to_string(index));

    // write message to file
    newMsgFile << subject << "\n";
    newMsgFile << msg;
    newMsgFile.close();
}

std::string Logic::getSubject(std::string filepath)
{
    std::ifstream file(filepath);

    if(!file.is_open())
        return "ERR\n";

    std::string subject;

    std::getline(file, subject);

    file.close();
    return subject;
}

std::string Logic::getBody(std::string filepath)
{
    std::ifstream file(filepath);
    std::string temp;
    std::stringstream msg;

    if(!file.is_open())
        return "ERR\n";

    // skip first line (subject)
    std::getline(file, temp);

    // read rest of file
    msg << file.rdbuf();
    file.close();
    return msg.str();
}
