#include <iostream>
#include <fstream>
#include <vector>
#include "ssem.h"

int main(int argc, char* argv[]) {
    // Check for command line arguments
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
        std::cout << "Failed to open file " << file_name << "\n";
        return 1;
    }

    // Declare vector which stores data as uint32 in Little Endian format
    std::vector<std::string> data;

    // Line to read the file into
    std::string line;
    while (getline(fs, line)) {
        // Check if the line is 32 bits long
        if(line.length() != 32) {
            std::cout << "LINE: " << line << "\n";
            std::cout << "Is not 32 bits long\n";
            return 1;
        }

        // Check if the line contains just 0's and 1's
        for(char &c: line) {
            if(!(c == '0' || c == '1')) {
                std::cout << "LINE: " << line << "\n";
                std::cout << "Contains non-binary digit\n";
                exit(1);
            }
        }

        data.push_back(line);
    }


    // Init SSEM with with data
    SSEM mb = SSEM(data);

    // Start the emulator
    mb.start();

    // Dump state afterwards
    mb.dump_state();

    return 0;
}