//
// Created by Andrius Navasaitis on 07/11/2018.
//

#include "assembler.h"
#include <regex>
#include <iostream>
#include <map>
#include <bitset>

Assembler::Assembler() = default;
Assembler::~Assembler() = default;

// ^([A-Z]\w*\:)?\s+(VAR|NUM|JMP|JRP|LDN|STO|SUB|CMP|STP)\s+(\w+)$
// ^([A-Z]\w*\:)?\s+(VAR|NUM|JMP|JRP|LDN|STO|SUB|CMP|STP)\s+(\w+)(?:;.*)?$
std::map<std::string, unsigned long int> labels;
//std::map<std::string, std::tuple<unsigned long int, unsigned long int>> vars;

//std::string regexpString = R"(^([A-Z]\\w*\\:)?\\s+(VAR|NUM|JMP|JRP|LDN|STO|SUB|CMP|STP)\\s+(\\w+)(?:;.*)?$))";
// ^(?:([A-Z]\w*):)?\s*(VAR|NUM|JMP|JRP|LDN|STO|SUB|CMP|STP)\s*(\w+)\s*(?:;.*)?$

std::regex re(R"(^(?:([A-Z]\w*):)?\s*(VAR|JMP|JRP|LDN|STO|SUB|CMP|STP)(?:$|\s+(-?\w+)?\s*)(?:;.*)?$)");

enum regexGroups {
    LBL = 1,
    CMD,
    OP,
};

void passOne(std::vector<std::string> data) {
    std::smatch match;

    unsigned long int lineNumber = 0;

    for(auto &line: data) {
        if(std::regex_search(line, match, re) && match.size() > 1) {
            if(!match.str(LBL).empty()) {

//                if(match.str(CMD) == "STP") {
//                    labels.insert(std::pair<std::string, int>(match.str(LBL), lineNumber));
//                    continue;
//                }

                // Check if operand is present
//                if(match.str(OP).empty()) {
//
//                    // TODO: Throw error
//                    std::cout << "ERROR: Missing operand after command " << match.str(CMD) << "\n" << line << "\n";
//                    exit(1);
//                }

//                if(match.str(CMD) == "VAR") {
//                    if(match.str(OP).empty()) {
//                        std::cout << "ERROR: Missing operand after VAR " << match.str(CMD) << "\n" << line << "\n";
//                        exit(1);
//                    }
//
////                    vars.insert(
////                            std::pair<std::string, std::tuple<unsigned long int, unsigned long int>>(
////                            match.str(LBL), std::make_tuple(std::stoul(match.str(OP), nullptr), lineNumber)));
//
//                    labels.insert(std::pair<std::string, unsigned long int>(match.str(LBL), lineNumber));
//                } else {
//                    labels.insert(std::pair<std::string, unsigned long int>(match.str(LBL), lineNumber));
//                }
                labels.insert(std::pair<std::string, unsigned long int>(match.str(LBL), lineNumber));
            }
        } else {
            std::cout << "Line does not match pattern: {LABEL: COMMAND OPERAND}\n" <<
            "LABEL: (optional) Must be all capital letters, must be [A-Z][A-Z0-9]+ format\n" <<
            "COMMAND (required) Must be VAR|JMP|JRP|LDN|STO|SUB|CMP|STP\n" <<
            "OPERAND (optional with STP) Can be a variable name, or a 32bit number" <<
            "\n" << line << "\n";
            exit(1);
        }
        lineNumber++;
    }
}

const std::map<std::string, unsigned long int> bitMap = {
        {"JMP", 0x0},
        {"JRP", 0x2000},
        {"LDN", 0x4000},
        {"STO", 0x6000},
        {"SUB", 0x8000},
        {"CMP", 0xC000},
        {"STP", 0xE000},
 };

std::vector<std::string> passTwo(std::vector<std::string> data) {
    std::smatch match;

    std::vector<std::string> output;

    unsigned long int lineNumber = 0;

    for(auto &line: data){
        std::cout << line << "\n";
        if(std::regex_search(line, match, re) && match.size() > 1) {
            unsigned long int bitLine = 0;

            const std::string cmd = match.str(CMD);
            std::string op = match.str(OP);

            unsigned long int ln = std::bitset<5>(lineNumber).to_ulong();

            // Check if command is STP
            if(cmd == "STP" || cmd == "CMP") {
                bitLine |= bitMap.find(cmd)->second;
//                bitLine |= ln;
                goto end;
            }

            if(op.empty()) {
                std::cout << "ERROR: Missing operand after: " << match.str(CMD) << "\n" << line << "\n";
                exit(1);
            }

            if(cmd == "VAR") {
                try {
                    bitLine |= stol(op, nullptr);
                    goto end;
                } catch (std::invalid_argument &err) {
                    std::cout << "ERROR: Operand is not an integer number\n" << line << "\n";
                    exit(1);
                }
            }

            if(labels.count(op)) {
                ln = labels.find(op)->second;
                bitLine |= bitMap.find(cmd)->second;
                bitLine |= ln;
            } else {
                try {

                    long int opNumber = stol(op, nullptr);
//                    long int number = std::bitset<32>(opNumber).to;
                    bitLine |= bitMap.find(cmd)->second;
                    bitLine |= opNumber;

                } catch(std::invalid_argument &err) {
                    std::cout << "ERROR: Operand is not an integer number\n" << line << "\n";
                    exit(1);
                }
            }




//            if(cmd == "STP") {
//                bitLine |= bitMap.find("STP")->second;
//                bitLine |= ln;
//            }else if(cmd == "JMP") {
//                ln = labels.find(op)->second;
//                bitLine |= bitMap.find("JMP")->second;
//                bitLine |= ln;
//            } else {
//                if(labels.count(op)) {
////                    std::tuple<unsigned long int, unsigned long int> tp = vars.find(op)->second;
////                    unsigned long int number = std::get<0>(tp);
////                    ln = std::get<1>(tp);
//                    bitLine |= bitMap.find(cmd)->second;
//                    bitLine |= labels.find(op)->second;
//
//                } else {
////                    std::cout << "CMD: " << cmd << " OP: " << op << "\n";
////
////                    for(auto &item: vars) {
////                        std::cout << "KEY: " << item.first << " VALUE: " << item.second << "\n";
////                    }
//
//                    try {
//                        unsigned long int opNumber = stoul(op, nullptr);
//                        unsigned long int number = std::bitset<32>(opNumber).to_ulong();
//                        bitLine |= number;
//
//                    } catch(std::invalid_argument &err) {
//                        std::cout << "Label: " << op << " was not found, terminating" << "\n";
//                        exit(1);
//                    }
//                }
//            }

            end:

            std::string out = std::bitset<32>(bitLine).to_string();
            std::reverse(out.begin(), out.end());
            std::cout << out << "\n";

            output.push_back(out);

        } else {
            std::cout << "Line does not match pattern: {LABEL: COMMAND OPERAND}\n" <<
                      "LABEL: (optional) Must be all capital letters, must be [A-Z][A-Z0-9]+ format\n" <<
                      "COMMAND (required) Must be VAR|JMP|JRP|LDN|STO|SUB|CMP|STP\n" <<
                      "OPERAND (optional with STP) Can be a variable name, or a 32bit number" <<
                      "\n\n" << line << "\n";
            exit(1);
        }
        lineNumber++;
    }

    return output;
}

std::string Assembler::trimWhiteSpaces(const std::string &s) {
    // Trim leading whitespaces
    std::string line = std::regex_replace(s, std::regex("^\\s+"), std::string(""));

    // Trim trailing whitespaces
    return std::regex_replace(line, std::regex("\\s+$"), std::string(""));
}


std::vector<std::string> Assembler::assemble(std::ifstream &fs) {

    // Init output vector
    std::vector<std::string> input;
    std::string line;


    unsigned int ln = 0;
    // Get labels and variables
    while(getline(fs, line)) {

        // Erase whitespaces
        line = trimWhiteSpaces(line);

        // Ignore empty lines
        if(line.empty()) {
            continue;
        }

        if(line.find(';', 0) != 0) {
            input.push_back(line);
        }
    }

    passOne(input);
    std::vector<std::string> output = passTwo(input);

    return output;
}


//^(?:[A-Z][A-Z0-9]+\:?) (VAR|NUM|JMP|JRP|LDN|STO|SUB|CMP|STP) (([0-9]+)|([A-Z0-9]+))$

/**
; THIS PROGRAM ADDS TWO NUMBERS TOGETHER
; The program starts at address 1, so we need to pad out address 0
          VAR 0       ; Declare 32-bit variable to fill space
START:    LDN NUM01   ; Copy variable to accumulator (negated)
          SUB NUM02   ; Subtract variable from accumulator to get sum (negated)
          STO MYSUM   ; Store accumulator to variable - saves answer
; The next two instructions negate the stored answer
          LDN MYSUM   ; Copy variable to accumulator (negated)
          STO MYSUM   ; Store accumulator to variable
END:      STP         ; Stop processor
; Variable declarations follow
NUM01:    VAR 1025    ; Declare 32-bit variable
NUM02:    VAR 621     ; Declare 32-bit variable
MYSUM:    VAR 0       ; Declare 32-bit variable
**/