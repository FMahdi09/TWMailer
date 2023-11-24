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
    std::string lineText; //Text to print as help for user what to enter
    
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
    return sendText;                    //assembled message will be returned
}

std::string MessageInput::inputRead()
{
    std::string sendText = "READ\n";
    std::string line;
    std::string lineText;

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
    std::string lineText;

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

std::string MessageInput::inputLogin(std::string& username)
{
    std::string sendText = "LOGIN\n";
    std::string line;
    
    std::string lineText = "<LDAP Username> ";
    std::cout << lineText;
    getline(std::cin, line);
    sendText = sendText + line + "\n";

    lineText = "<Password> ";
    std::cout << lineText;
    line = readPassword();
    sendText = sendText + line + "\n";

    return sendText;                    //assembled message will be returned
}


std::string MessageInput::readPassword()
{
    int show_asterisk = 0;

    const char BACKSPACE = 127;
    const char RETURN = 10;

    unsigned char ch = 0;
    std::string password;

    printf("Password: ");

    while ((ch = getch()) != RETURN)
    {
        if (ch == BACKSPACE)
        {
            if (password.length() != 0)
            {
                if (show_asterisk)
                {
                    printf("\b \b"); // backslash: \b
                }
                password.resize(password.length() - 1);
            }
        }
        else
        {
            password += ch;
            if (show_asterisk)
            {
                printf("*");
            }
        }
    }
    printf("\n");
    return password;
}


int MessageInput::getch()
{
    int ch;
    // https://man7.org/linux/man-pages/man3/termios.3.html
    struct termios t_old, t_new;

    // https://man7.org/linux/man-pages/man3/termios.3.html
    // tcgetattr() gets the parameters associated with the object referred
    //   by fd and stores them in the termios structure referenced by
    //   termios_p
    tcgetattr(STDIN_FILENO, &t_old);

    // copy old to new to have a base for setting c_lflags
    t_new = t_old;

    // https://man7.org/linux/man-pages/man3/termios.3.html
    //
    // ICANON Enable canonical mode (described below).
    //   * Input is made available line by line (max 4096 chars).
    //   * In noncanonical mode input is available immediately.
    //
    // ECHO   Echo input characters.
    t_new.c_lflag &= ~(ICANON | ECHO);

    // sets the attributes
    // TCSANOW: the change occurs immediately.
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    // reset stored attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);

    return ch;
}

