#ifndef MANCHESTERBABY_SSEM_H
#define MANCHESTERBABY_SSEM_H

#include "store.h"
#include "cpu.h"


class SSEM {
    public:
        explicit SSEM(std::vector<std::string> input);
        ~SSEM();

        void start();
        void dump_state();
        static std::string assemble(unsigned int line);

    private:

        Store store;
        CPU cpu;
};


#endif //MANCHESTERBABY_SSEM_H
