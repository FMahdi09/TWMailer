#include "messageInput.h"

// ctor
MessageInput::MessageInput()
{

}

// public functions

std::string MessageInput::inputSend()
{
    std::string sendText = "SEND\n";
    std::string line;
    std::string lineText = "<Sender> ";

    //sendText += lineText;
    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";
    
    lineText = "<Receiver> ";
    std::cout << lineText;
    //sendText += lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";

    lineText = "<Subject> ";
    std::cout << lineText << "(max. 80 Characters) ";
    //sendText += lineText;
    getline(std::cin, line);
    while(line.length() > 80)
    {
        std::cout << lineText;
        std::cout << lineText;
        getline(std::cin, line);
    }
    sendText = sendText + line + "\n";

    line = " ";
    lineText = "<message> ";
    std::cout << lineText << "(a single dot ends the message) ";
    //sendText += lineText;
    while(line != ".")
    {
        getline(std::cin, line);
        sendText = sendText + line + "\n";
    }

    /*std::cout << "___" << std::endl;
    std::cout << sendText << std::endl;
    std::cout << "___" << std::endl;*/
    return sendText;
}

std::string MessageInput::inputList()
{
    std::string sendText = "LIST\n";
    std::string line;
    std::string lineText = "<Username> ";

    //sendText += lineText;
    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";
    return sendText;
}

std::string MessageInput::inputRead()
{
    std::string sendText = "READ\n";
    std::string line;
    std::string lineText = "<Username> ";

    //sendText += lineText;
    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";

    lineText = "<Message-Number> ";
    //sendText += lineText;
    std::cout << lineText;
    getline(std::cin, line);

    char* p;
    strtol(line.c_str(), &p, 10);
    
    if(*p == 0)
    {
        std::cout << "ja" << std::endl;
    }
    else
    {
        std::cout << "nein" << std::endl;
    }
    while(*p != 0)
    {
        
    }

    sendText = sendText + line + "\n";
    return sendText;
}

std::string MessageInput::inputDel()
{

    return "a";
}
