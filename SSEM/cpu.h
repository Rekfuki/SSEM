//
// Created by Andrius Navasaitis on 06/11/2018.
//

#ifndef MANCHESTERBABY_CPU_H
#define MANCHESTERBABY_CPU_H


#include "store.h"

enum CPU_STATE {
    FETCH,
    EXECUTE,
    HALT,
};

class CPU {

    public:

        explicit CPU(Store &store);
        ~CPU();

        void fetch();
        void execute();
        void halt();
        void run();

//        void dump_state();
//        std::string assemble(unsigned int line);

        unsigned int get_ac();
        unsigned int get_ci();
        unsigned int get_pi();

private:

        Store &store;

        CPU_STATE cpu_state;

        unsigned int accumulator;
        unsigned int current_instruction;
        unsigned int present_instruction;

};


#endif //MANCHESTERBABY_CPU_H
