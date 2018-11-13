#include <iostream>


#include "assembler.h"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Unsupported amount of arguments: " << argc << "\n";
        std::cout << "Please provide only the file name" << "\n";
        return 1;
    }

    // Get the file name
    std::string file_name = argv[1];

    std::cout << "Opening file " << file_name << std::endl;

    // Try to open the file
    std::ifstream fs(file_name);

    // check if the file is open
    if (!fs.is_open()) {
        std::cout << "Failed to open file " << file_name << std::endl;
        return 1;
    }

    // Construct the assembler
    Assembler assembler;

    // get the output
    std::vector<std::string> output = assembler.assemble(fs);

    std::ofstream outFile("assembled.txt");

    for (const auto &o : output){
        outFile << o << "\n";
    }

    return 0;
}