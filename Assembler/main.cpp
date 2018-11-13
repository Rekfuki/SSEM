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

    std::cout << "\nOpening file " << file_name << "\n\n";

    // Try to open the file
    std::ifstream fs(file_name);

    // Check if the file is open
    if (!fs.is_open()) {
        std::cout << "Failed to open file " << file_name << "\n";
        return 1;
    }

    // Construct the assembler
    Assembler assembler;

    // Get the output
    std::vector<std::string> output = assembler.assemble(fs);

    std::ofstream outFile("assembled.txt");

    // Write to file
    for (const auto &o : output){
        outFile << o << "\n";
    }

    std::cout << "\nDone.\nMachine code can be found in assembled.txt\n";
    return 0;
}