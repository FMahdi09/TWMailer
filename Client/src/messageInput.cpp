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

    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";
    
    lineText = "<Receiver> ";
    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";

    lineText = "<Subject> ";
    std::cout << lineText << "(max. 80 Characters) ";
    getline(std::cin, line);
    while(line.length() > 80)
    {
        std::cout << "Subject too many characters. Please enter a shorter subject." << std::endl;
        std::cout << lineText << "(max. 80 Characters) ";
        getline(std::cin, line);
    }
    sendText = sendText + line + "\n";

    line = " ";
    lineText = "<message> ";
    std::cout << lineText << "(a single dot ends the message) ";
    while(line != ".")
    {
        getline(std::cin, line);
        sendText = sendText + line + "\n";
    }
    return sendText;
}

std::string MessageInput::inputList()
{
    std::string sendText = "LIST\n";
    std::string line;
    std::string lineText = "<Username> ";

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

    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";

    lineText = "<Message-Number> ";
    std::cout << lineText;
    getline(std::cin, line);

    char* p;
    strtol(line.c_str(), &p, 10);

    while(*p != 0)
    {
        std::cout << "This is not a valid number. Please try again." << std::endl;
        std::cout << lineText;
        getline(std::cin, line);

        strtol(line.c_str(), &p, 10);
    }

    sendText = sendText + line + "\n";
    return sendText;
}

std::string MessageInput::inputDel()
{
    std::string sendText = "DEL\n";
    std::string line;
    std::string lineText = "<Username> ";

    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";

    lineText = "<Message-Number> ";
    std::cout << lineText;
    getline(std::cin, line);

    char* p;
    strtol(line.c_str(), &p, 10);

    while(*p != 0)
    {
        std::cout << "This is not a valid number. Please try again." << std::endl;
        std::cout << lineText;
        getline(std::cin, line);

        strtol(line.c_str(), &p, 10);
    }

    sendText = sendText + line + "\n";
    return sendText;
}
