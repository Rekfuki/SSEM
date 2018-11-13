#include <iostream>
#include <fstream>
#include <vector>
#include "SSEM/ssem.h"
#include "Assembler/assembler.h"

int main(int argc, char* argv[]) {
    // Check for command line arguments
    if(argc != 2) {
        std::cout << "Unsupported amount of arguments: " << argc << std::endl;
        std::cout << "Please provide only the file name" << std::endl;
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

    // Declare vector which stores data as uint32 in Little Endian format
    std::vector<std::string> data;

    // Line to read the file into
    std::string line;
    while (getline(fs, line)) {
        // Check if the line is 32 bits long
        if(line.length() != 32) {
            std::cout << "File contains line that is not 32 bits long" << std::endl;
            return 1;
        }

        data.push_back(line);
    }


    // Init SSEM with with data
    SSEM mb = SSEM(data);

    // start the emulator
    mb.start();

    // dump state afterwards
    mb.dump_state();

    return 0;
}