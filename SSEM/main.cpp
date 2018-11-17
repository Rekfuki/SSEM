#include <iostream>
#include <fstream>
#include <vector>
#include "ssem.h"

int main(int argc, char* argv[]) {
    // Check for command line arguments
    if(argc < 2 || argc > 3) {
        std::cout << "Unsupported amount of arguments: " << argc - 1<< "\n";
        std::cout << "Please provide only the flag -s for step mode (optional) and the file name" << "\n";
        return 1;
    }

    std::vector<std::string> argList(argv + 1, argv + argc);

    // Step mode flag
    bool step_mode = false;

    // File name
    std::string file_name;

    switch (argList.size()) {
        case 2:

            if(argList.at(0) != "-s") {
                std::cout << "\nWhen providing the flag -s, the flag has to go before the file name\n";
                return 1;
            }

            if(argList.at(1).find(".txt") == std::string::npos) {
                std::cout << "\nFile has to end with a .txt extension\n";
                return 1;
            }

            step_mode = true;
            file_name = argList.at(1);

            break;
        case 1:
            if(argList.at(0).find(".txt") == std::string::npos) {
                std::cout << "\nFile has to end with a .txt extension\n";
                return 1;
            }

            file_name = argList.at(0);
            break;
        default:
            return 1;
    }

    std::cout << "Opening file {" << file_name << "}\n";

    // Try to open the file
    std::ifstream fs(file_name);

    // check if the file is open
    if (!fs.is_open()) {
        std::cout << "Failed to open file {" << file_name << "}\n";
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
    mb.start(step_mode);

    // Dump state afterwards
    mb.dump_state();

    return 0;
}