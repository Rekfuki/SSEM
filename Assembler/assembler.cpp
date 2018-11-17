#include "assembler.h"
#include <iostream>
#include <regex>
#include <bitset>

// Assembler constructor
Assembler::Assembler() = default;

// Assembler destructor
Assembler::~Assembler() = default;

// Regex pattern to match the assembly code
std::regex re(R"(^(?:([A-Z]\w*):)?\s*(VAR|JMP|JRP|LDN|STO|SUB|CMP|STP)(?:$|\s+(-?\w+)?\s*)(?:;.*)?$)");

// Maps all the labels
void Assembler::passOne(std::vector<std::string> data) {
    std::cout << "Starting first pass" << "\n\n";
    std::smatch match;

    unsigned long int lineNumber = 0;

    // Loop through the vector
    for(auto &line: data) {
        // Check if the line matches regex pattern
        if(std::regex_search(line, match, re) && match.size() > 1) {
            // If the line does not contain a label we ignore it
            if(!match.str(LBL).empty()) {
                std::cout << "Found label: " << match.str(LBL) << "\n";

                // Insert the label and the label line number into the symbol table
                labels.insert(std::pair<std::string, unsigned long int>(match.str(LBL), lineNumber));
            }
        } else {
            std::cout << "LINE: " << line <<"\nDoes not match pattern: {LABEL: COMMAND OPERAND}\n" <<
            "\nLABEL: (optional) Must be all capital letters, must be [A-Z][A-Z0-9]+ format\n" <<
            "COMMAND (required) Must be VAR|JMP|JRP|LDN|STO|SUB|CMP|STP\n" <<
            "OPERAND (optional with STP and CMP) Can be a variable name, or a 32bit number" <<
            "\n";
            exit(1);
        }
        lineNumber++;
    }
}

// Maps instructions to binary representation in hex
const std::map<std::string, unsigned long int> bitMap = {
        {"JMP", 0x0000},    // 0000 0000 0000 0000 0000 0000
        {"JRP", 0x2000},    // 0000 0000 0010 0000 0000 0000
        {"LDN", 0x4000},    // 0000 0000 0100 0000 0000 0000
        {"STO", 0x6000},    // 0000 0000 0110 0000 0000 0000
        {"SUB", 0x8000},    // 0000 0000 1000 0000 0000 0000
        {"CMP", 0xC000},    // 0000 0000 1100 0000 0000 0000
        {"STP", 0xE000},    // 0000 0000 1110 0000 0000 0000
 };

// Assembles machine code based on symbol table
std::vector<std::string> Assembler::passTwo(std::vector<std::string> data) {
    std::cout << "\n\nStarting second pass" << "\n\n";
    std::smatch match;

    // Declare an ouput vector
    std::vector<std::string> output;

    unsigned long int lineNumber = 0;

    // Loop through the machine code
    for(auto &line: data){
        std::cout << line << "\n";

        // Check if the line matches the regex pattern
        if(std::regex_search(line, match, re) && match.size() > 1) {
            unsigned long int bitLine = 0;

            // Get the command and the operand
            const std::string cmd = match.str(CMD);
            std::string op = match.str(OP);

            // Check if command is STP or CMP
            // Special case since they are not followed by an opperand
            if(cmd == "STP" || cmd == "CMP") {

                // Set the command bits
                bitLine |= bitMap.find(cmd)->second;
                goto end;
            }

            // If not CMP or STP the instruction has to have an operand
            if(op.empty()) {
                std::cout << "\nERROR: Missing operand after: " << match.str(CMD) << "\n";
                exit(1);
            }

            // If the instruction is VAR
            // Try to convert it
            if(cmd == "VAR") {
                try {
                    bitLine |= stol(op, nullptr);
                    goto end;
                } catch (std::invalid_argument &err) {
                    std::cout << "\nERROR: Operand is not an integer number\n";
                    exit(1);
                }
            }

            // If the label is in the symbol table
            if(labels.count(op)) {

                // Get the label line number from the symbol table
                unsigned long int ln = labels.find(op)->second;

                // Set the command bits
                bitLine |= bitMap.find(cmd)->second;

                // Set the line bits
                bitLine |= ln;
            } else {
                try {
                    // Since the operand has to be a number at this point

                    // Try to convert the operand
                    long int opNumber = stol(op, nullptr);

                    // Set the command bits
                    bitLine |= bitMap.find(cmd)->second;

                    // Set the line bits
                    bitLine |= opNumber;

                } catch(std::invalid_argument &err) {
                    std::cout << "\nERROR: Operand was not found in the symbol table and is not an integer number\n";
                    exit(1);
                }
            }

            // Label to jump from specific cases
            // Possibly not the best design
            end:

            // Convert the bit line into a 32 bit line
            std::string out = std::bitset<32>(bitLine).to_string();

            // Reverse the line since the manchester baby takes reveresed bit significance
            std::reverse(out.begin(), out.end());
            std::cout << out << "\n";

            // Add the data to the vector
            output.push_back(out);

        } else {
            std::cout << "LINE: "<< line << "Does not match pattern: {LABEL: COMMAND OPERAND}\n" <<
                      "\nLABEL: (optional) Must be all capital letters, must be [A-Z][A-Z0-9]+ format\n" <<
                      "COMMAND (required) Must be VAR|JMP|JRP|LDN|STO|SUB|CMP|STP\n" <<
                      "OPERAND (optional with STP and CMP) Can be a variable name, or a 32bit number" <<
                      "\n\n";
            exit(1);
        }
        lineNumber++;
    }

    return output;
}

// Used to trim white spaces
// C++ has no nice way of doing it, hence regexp to the help
std::string Assembler::trimWhiteSpaces(const std::string &s) {
    // Trim leading whitespaces
    std::string line = std::regex_replace(s, std::regex("^\\s+"), std::string(""));

    // Trim trailing whitespaces
    return std::regex_replace(line, std::regex("\\s+$"), std::string(""));
}

// Assembles the machine code
std::vector<std::string> Assembler::assemble(std::ifstream &fs) {

    // Init output vector
    std::vector<std::string> input;
    std::string line;


    // Get labels and variables
    while(getline(fs, line)) {

        // Erase whitespaces
        line = trimWhiteSpaces(line);

        // Ignore empty lines
        if(line.empty()) {
            continue;
        }

        // If the line does not begin with a ';'
        // Ignoring comments
        if(line.find(';', 0) != 0) {
            input.push_back(line);
        }
    }

    // Do the first pass to collect all the labels
    passOne(input);

    // Do pass two to retrieve the assembled machine code
    std::vector<std::string> output = passTwo(input);

    // Print out the assembled file
    std::cout << "\nASSEMBLED FILE: \n";
    for(auto &ln: output) {
        std::cout << ln << "\n";
    }

    return output;
}
