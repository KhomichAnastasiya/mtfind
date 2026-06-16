#include <iostream>

#include "mtfind.h"
#include "tests.h"


int main(int argc, char* argv[])
{
    if (argc >= 2 && std::string(argv[1]) == "--test")
    {
        RunAllTests();
        return 0;
    }

    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <filename> <pattern>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string pattern = argv[2];

    MtFind mtFind;

    if (mtFind.loadAndSearch(filename, pattern))
    {
        mtFind.printResults();
        return 0;
    }
    else
    {
        std::cout << "0" << std::endl;
        return 0;
    }
    return 0;
}
