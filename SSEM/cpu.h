#ifndef MANCHESTERBABY_CPU_H
#define MANCHESTERBABY_CPU_H


#include "store.h"


// Enumeration of the cpu states for readability
enum CPU_STATE {
    FETCH,
    EXECUTE,
    HALT,
};

class CPU {

    public:

        explicit CPU(Store &store); // Constructor
        ~CPU();                     // Destructor

        void fetch();               // Fetch instruction
        void execute();             // Execute instruction
        void run();                 // Run the cpu

        // Getters
        unsigned int get_ac();
        unsigned int get_ci();
        unsigned int get_pi();

private:

        Store &store;                       // Store object

        CPU_STATE cpu_state;                // CPU state

        unsigned int accumulator;           // Accumulator
        unsigned int current_instruction;   // Current instruction
        unsigned int present_instruction;   // Present instruction

        void print_control();               // Prints the control

};


#endif //MANCHESTERBABY_CPU_H
