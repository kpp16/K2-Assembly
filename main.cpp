#include "include/Parser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cout << "Usage: K2Assembly <filePath>" << std::endl;
        return EXIT_FAILURE;
    }

    const char* filePath = argv[1];

    std::string str(filePath);
    Parser parser(str);

    parser.interpret();

    return 0;
}