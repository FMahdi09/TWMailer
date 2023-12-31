#include <iostream>
#include <string>
#include <memory>
#include <signal.h>

#include "server.h"

int main(int argc, char* argv[])
{
    // check validity of arguments:

    // 2 parameters
    if(argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <port> <mail-spool-directoryname>\n";
        return EXIT_FAILURE;
    }

    try
    {
        // 1st parameter (port) is a valid number
        int port = std::stoi(argv[1]);

        // initialize server
        Server server(port, argv[2]);

        // start main loop
        server.start();
    }
    catch (std::invalid_argument const& ex)
    {
        std::cerr << "<port>:\n"
                  << "expected: number, provided: " << argv[1] << "\n";
        return EXIT_FAILURE;
    }
    catch(std::out_of_range const& ex)
    {
        std::cerr << "<port>:\n"
                  << "invalid port, provided: " << argv[1] << "\n";
        return EXIT_FAILURE;
    }
    catch(std::exception const& ex)
    {
        std::cerr << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
