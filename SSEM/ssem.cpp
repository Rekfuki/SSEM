#include <vector>
#include <iostream>
#include <stdio.h>

#include "ssem.h"


// SSEM constructor
SSEM::SSEM(std::vector<std::string> input) : cpu(store) {
    store.process_data(std::move(input));
}

// SSEM destructor
SSEM::~SSEM() = default;

// Dump the state after the execution
void SSEM::dump_state() {
    printf("\n------------------- SSE Machine State -------------------\n\n");

    for(unsigned int i = 0; i < store.get_data().size(); i++) {

        // Get the line from the store
        unsigned int line = store.line_at(i);

        // Create bit string from unsigned int
        std::string bits = std::bitset<32>(line).to_string();

        // Reverse the string back to least significant bit
        std::reverse(bits.begin(), bits.end());

        // Format memory line to assembly output
        std::string assembly = assemble(store.line_at(i));

        // print pretty format
        // ln: hex - store line - assembly - integer value
        printf("%02d: 0x%08x - %s %s\t%d\n", i, line, bits.c_str(),
                 assembly.c_str(), line);
    }

    unsigned int accumulator = cpu.get_ac();
    unsigned int current_instruction = cpu.get_ci();
    unsigned int present_instruction = cpu.get_pi();

    // Present accumulator, ci and pi in 32 bit binary string
    std::string ac_bits = std::bitset<32>(accumulator).to_string();
    std::reverse(ac_bits.begin(), ac_bits.end());

    std::string ci_bits = std::bitset<32>(current_instruction).to_string();
    std::reverse(ci_bits.begin(), ci_bits.end());

    std::string pi_bits = std::bitset<32>(present_instruction).to_string();
    std::reverse(pi_bits.begin(), pi_bits.end());

    printf("\nAC: 0x%08x - %s %d\nCI: 0x%08x - %s %d\nPI: 0x%08x - %s %s\n",
           accumulator, ac_bits.c_str(), accumulator,
           current_instruction, ci_bits.c_str(), current_instruction,
           present_instruction, pi_bits.c_str(), assemble(present_instruction).c_str());
}

// Assembles machine code into assembler instruction
std::string SSEM::assemble(unsigned int line) {
    // Declare a buffer to sprintf into
    char out[32];

    // Checking if the bits 6-12 are set
    // If they are, we assume it is a variable and not an instruction
    if((line & 0x1FE0) > 0) {
        sprintf(out, "NUM %2d", static_cast<int>(line));
        return static_cast<std::string>(out);
    }

    // Get the line number
    int data = static_cast<int>(line & 0x1F);

    // switch based on bits set by using 111 mask
    switch(line & 0xE000) {
        case 0x0000:     // 0000 0000 0000 0000 0000 0000    0x0000
            sprintf(out, "JMP %2d", data);
            return static_cast<std::string>(out);
        case 0x2000:     // 0000 0000 0010 0000 0000 0000    0x2000
            sprintf(out, "JRP %2d", data);
            return static_cast<std::string>(out);
        case 0x4000:    // 0000 0000 0100 0000 0000 0000    0x4000
            sprintf(out, "LDN %2d", data);
            return static_cast<std::string>(out);
        case 0x6000:    // 0000 0000 0110 0000 0000 0000    0x6000
            sprintf(out, "STO %2d", data);
            return static_cast<std::string>(out);
        case 0x8000:    // 0000 0000 1000 0000 0000 0000    0x8000:
            sprintf(out, "SUB %2d", data);
            return static_cast<std::string>(out);
        case 0xC000:    // 0000 0000 1100 0000 0000 0000    0xC000
            // Adding 3 extra spaces because of %2d to retain pretty format
            return "CMP   ";
        case 0xE000:    // 0000 0000 1110 0000 0000 0000    0xE000
            return "STP   ";
        default:
            return "";
    }
}

// Starts the processor cycle
void SSEM::start(bool step_mode) {
    cpu.run(step_mode);
}
