#include <iostream>
#include <csignal>
#include "cpu.h"
#include "ssem.h"


// CPU constructor
CPU::CPU(Store &store) : store(store), cpu_state(HALT),
accumulator(0),current_instruction(0), present_instruction(0){
}

// CPU destructor
CPU::~CPU() = default;

// Fetch instruction
void CPU::fetch() {
    // Increment the CI
    current_instruction++;

    // Get the PI from store at current instructions line number
    present_instruction = store.line_at(current_instruction & 0x1F);
}

// 0000 0000 0000 0000 0000 0000    0x0000
// 0000 0000 0010 0000 0000 0000    0x2000
// 0000 0000 0100 0000 0000 0000    0x4000
// 0000 0000 0110 0000 0000 0000    0x6000
// 0000 0000 1000 0000 0000 0000    0x8000
// 0000 0000 1100 0000 0000 0000    0xC000
// 0000 0000 1110 0000 0000 0000    0xE000

// Execute instruction
void CPU::execute() {

    // get the line of the PI
    unsigned int data = present_instruction & 0x1F;

    switch(present_instruction & 0xE000) {
        case 0x0000:    // 0000 0000 0000 0000 0000 0000    0x0000
            current_instruction = store.line_at(data);
            break;
        case 0x2000:    // 0000 0000 0010 0000 0000 0000    0x2000
            current_instruction += store.line_at(data);
            break;
        case 0x4000:    // 0000 0000 0100 0000 0000 0000    0x4000
            accumulator = (-1) * store.line_at(data);
            break;
        case 0x6000:    // 0000 0000 0110 0000 0000 0000    0x6000
            store.set_data(data, accumulator);
            break;
        case 0x8000:    // 0000 0000 1000 0000 0000 0000    0x8000
            accumulator -= store.line_at(data);
            break;
        case 0xC000:    // 0000 0000 1100 0000 0000 0000    0xC000
            if(accumulator < 0) {
                current_instruction++;
            }
            break;
        case 0xE000:    // 0000 0000 1110 0000 0000 0000    0xE000
            cpu_state = HALT;
            return;
        default:
            std::cout << "\nError unknown instruction\n";
            cpu_state = HALT;
            return;
    }
}

// Prints current cycle if step  mode is enabled
void CPU::step_m() {
    if(cpu_state == FETCH) {
        std::cout << "Fetched instruction: " << SSEM::assemble(present_instruction) << "\n" ;
    }

    if(cpu_state == EXECUTE) {
        std::cout << "Executed instruction: " << SSEM::assemble(present_instruction) << "\n" ;
    }
}

// Starts the cpu cycle
void CPU::run(bool step_mode) {

    // Check if the cpu initialization was correct
    if(cpu_state != HALT) {
        std::cout << "\nWrong CPU state\n";
        return;
    }

    // Set the state to fetch
    cpu_state = FETCH;

    // Print the first line of the control
    print_control();

    // While the cpu is not halted, continue with the cycle
    while(cpu_state != HALT) {

        // If step mode is enabled
        if(step_mode) {
            getchar();
        }

        switch (cpu_state) {

            case FETCH:

                fetch();
                step_m();
                cpu_state = EXECUTE;

                break;
            case EXECUTE:

                execute();
                step_m();

                if(cpu_state != HALT) {
                    cpu_state = FETCH;
                }

                print_control();

                break;
            default:
                std::cout << "\nWrong cpu state\n";
                return;
        }
    }
}

// Prints out the control
void CPU::print_control() {

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
           present_instruction, pi_bits.c_str(), SSEM::assemble(present_instruction).c_str());

}

// Get accumulator
unsigned int CPU::get_ac() {
    return accumulator;
}

// Get current instruction
unsigned int CPU::get_ci() {
    return current_instruction;
}

// Get present instruction
unsigned int CPU::get_pi() {
    return present_instruction;
}
