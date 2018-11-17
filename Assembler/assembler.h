#ifndef MANCHESTERBABY_ASSEMBLER_H
#define MANCHESTERBABY_ASSEMBLER_H

#include <fstream>
#include <vector>
#include <map>

class Assembler {

    // Enumeration to make it more clear which regex group is being referred to
    enum regexGroups {
        LBL = 1,
        CMD,
        OP,
    };

    std::map<std::string, unsigned long int> labels;    // Symbol table

    public:
        Assembler();    // Assembler constructor
        ~Assembler();   // Assembler destructor

        std::vector<std::string> assemble(std::ifstream &fs); // Assembles the provided asm file

    private:
        std::string trimWhiteSpaces(const std::string &s); // Removes leading and trailing whitespaces
        void passOne(std::vector<std::string> data); // First iteration to collect symbols
        std::vector<std::string> passTwo(std::vector<std::string> data); // Second iteration to process into machine code

};


#endif //MANCHESTERBABY_ASSEMBLER_H
