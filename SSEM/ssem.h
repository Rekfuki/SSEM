#ifndef MANCHESTERBABY_SSEM_H
#define MANCHESTERBABY_SSEM_H

#include "store.h"
#include "cpu.h"


class SSEM {
    public:
        explicit SSEM(std::vector<std::string> input);  // SSEM constructor
        ~SSEM();                                        // SSEM Destructor

        void start(bool step_mode);                     // Start the SSEM
        void dump_state();                              // Dump the state of the SSEM
        static std::string assemble(unsigned int line); // Assemble machine code into asm instruction

    private:

        Store store; // Store object
        CPU cpu;     // CPU object
};


#endif //MANCHESTERBABY_SSEM_H
