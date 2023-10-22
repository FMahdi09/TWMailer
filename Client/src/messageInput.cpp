#include "messageInput.h"

// ctor
MessageInput::MessageInput()
{

}

// public functions

std::string MessageInput::inputSend()
{
    std::string sendText;
    std::string line;
    std::string lineText = "<Sender> ";

    sendText = lineText;
    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";
    
    lineText = "<Receiver> ";
    std::cout << lineText;
    sendText += lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";

    lineText = "<Subject> ";
    std::cout << lineText << " (max. 80 Characters)";
    sendText += lineText;
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
    std::cout << lineText << " (a single dot ends the message)";
    sendText += lineText;
    while(line != ".")
    {
        getline(std::cin, line);
        sendText = sendText + line + "\n";
    }

    std::cout << "___" << std::endl;
    std::cout << sendText << std::endl;
    std::cout << "___" << std::endl;
    return sendText;
}

void MessageInput::inputList()
{

}

void MessageInput::inputRead()
{

}

void MessageInput::inputDel()
{

}