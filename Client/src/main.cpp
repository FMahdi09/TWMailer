#include <iostream>
#include <signal.h>

#include "client.h"

// signal handler
void signalHandler(int sig)
{
    if(sig == SIGPIPE)
        std::cerr << "server shutdown unexpecedly\n";

    exit(sig);
}

int main(int argc, char* argv[])
{
    // install signal handler
    if(signal(SIGPIPE, signalHandler) == SIG_ERR)
    {
        std::cerr << "signal handler can not be installed\n";
        return EXIT_FAILURE;
    }

    // check validity of arguments:

    // 2 parameters
    if(argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <ip> <port>\n";
        return EXIT_FAILURE;
    }

    try
    {
        // 2nd parameter (port) is a valid number
        int port = std::stoi(argv[2]);

        // initialize client
        Client client(port, argv[1]);

        // start main loop
        client.start();
    }
    catch (std::invalid_argument const& ex)
    {
        //if port entered is no valid number
        std::cerr << "<port>:\n"
                  << "expected: number, provided: " << argv[1] << "\n";
        return EXIT_FAILURE;
    }
    catch(std::out_of_range const& ex)
    {
        //if port entered is no valid number within allowed borders
        std::cerr << "<port>:\n"
                  << "invalid port provided: " << argv[1] << "\n";
        return EXIT_FAILURE;
    }
    catch(std::exception const& ex)
    {
        //catch of remaining exceptions and print of cause
        std::cerr << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return 0;
}
