#include "messageInput.h"

// ctor
MessageInput::MessageInput()
{

}

// public functions
//functions to assemble message to send dependent on command entered

std::string MessageInput::inputSend()
{
    std::string sendText = "SEND\n";    //all text to send later to the server
    std::string line;
    std::string lineText = "<Sender> "; //Text to print as help for user what to enter

    std::cout << lineText;
    getline(std::cin, line);            //get line of entered characters from cin and write in variable 'line'
    sendText = sendText + line + "\n";  //assemble text, which later is sent
    
    lineText = "<Receiver> ";           //set Helptext to current input expected
    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";

    lineText = "<Subject (max. 80 Characters)> ";
    std::cout << lineText;
    getline(std::cin, line);
    while(line.length() > 80)           //while as long as subject entered is longer than 80 characters
    {
        std::cout << "Subject too many characters. Please enter a shorter subject." << std::endl;
        std::cout << lineText << "(max. 80 Characters) ";
        getline(std::cin, line);
    }
    sendText = sendText + line + "\n";

    line = " ";
    lineText = "<message (a single dot ends the message)> ";
    std::cout << lineText;
    while(line != ".")                  //while as long as until a single dot in a line is entered as end of the message
    {
        getline(std::cin, line);
        sendText = sendText + line + "\n";
    }
    return sendText;                    //assembled message will be returned
}

std::string MessageInput::inputList()
{
    std::string sendText = "LIST\n";
    std::string line;
    std::string lineText = "<Username> ";

    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";
    return sendText;                    //assembled message will be returned
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
    strtol(line.c_str(), &p, 10);       //check if entered characters are a number

    while(*p != 0)                      //while as long as user had entered no number
    {
        std::cout << "This is not a valid number. Please try again." << std::endl;
        std::cout << lineText;
        getline(std::cin, line);

        strtol(line.c_str(), &p, 10);   //check if entered characters are a number
    }

    sendText = sendText + line + "\n";
    return sendText;                    //assembled message will be returned
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

    while(*p != 0)                      //while as long as user had entered no number
    {
        std::cout << "This is not a valid number. Please try again." << std::endl;
        std::cout << lineText;
        getline(std::cin, line);

        strtol(line.c_str(), &p, 10);   //check if entered characters are a number
    }

    sendText = sendText + line + "\n";
    return sendText;                    //assembled message will be returned
}
