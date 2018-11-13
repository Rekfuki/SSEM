//
// Created by Andrius Navasaitis on 03/11/2018.
//

#ifndef MANCHESTERBABY_SSEM_H
#define MANCHESTERBABY_SSEM_H

#include "store.h"
#include "cpu.h"


class SSEM {
    public:
        explicit SSEM(std::vector<std::string> input);
        ~SSEM();

        void start();
        void display_memory();
        void memory_string();
        void dump_state();

    private:

        std::string assemble(unsigned int line);

        Store store;
        CPU cpu;

//        unsigned int accumulator;
//        unsigned int current_instruction;
//        unsigned int present_instructionV;


};


#endif //MANCHESTERBABY_SSEM_H
