#include <iostream>

#include "client.h"

int main(int argc, char* argv[])
{
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
        std::cerr << "<port>:\n"
                  << "expected: number, provided: " << argv[1] << "\n";
        return EXIT_FAILURE;
    }
    catch(std::out_of_range const& ex)
    {
        std::cerr << "<port>:\n"
                  << "invalid port provided: " << argv[1] << "\n";
        return EXIT_FAILURE;
    }
    catch(std::exception const& ex)
    {
        std::cerr << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return 0;
}
