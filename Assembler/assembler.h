//
// Created by Andrius Navasaitis on 07/11/2018.
//

#ifndef MANCHESTERBABY_ASSEMBLER_H
#define MANCHESTERBABY_ASSEMBLER_H

#include <fstream>
#include <vector>

class Assembler {
    public:
        Assembler();
        ~Assembler();

        std::vector<std::string> assemble(std::ifstream &fs);

    private:
        std::string trimWhiteSpaces(const std::string &s);

};


#endif //MANCHESTERBABY_ASSEMBLER_H
