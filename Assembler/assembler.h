//
// Created by Andrius Navasaitis on 07/11/2018.
//

#ifndef MANCHESTERBABY_ASSEMBLER_H
#define MANCHESTERBABY_ASSEMBLER_H

#include <fstream>
#include <vector>
#include <map>

class Assembler {

    // regexGroups used to make it more clear which regex group is being refered to
    enum regexGroups {
        LBL = 1,
        CMD,
        OP,
    };

    std::map<std::string, unsigned long int> labels;

    public:
        Assembler();
        ~Assembler();

        std::vector<std::string> assemble(std::ifstream &fs);

    private:
        std::string trimWhiteSpaces(const std::string &s);
        void passOne(std::vector<std::string> data);
        std::vector<std::string> passTwo(std::vector<std::string> data);

};


#endif //MANCHESTERBABY_ASSEMBLER_H
