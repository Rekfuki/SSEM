#include <iostream>
#include <csignal>
#include "cpu.h"
#include "ssem.h"

CPU::CPU(Store &store) : store(store), cpu_state(HALT),
accumulator(0),current_instruction(0), present_instruction(0){
}

CPU::~CPU() = default;

void CPU::fetch() {
    current_instruction++;
    present_instruction = store.line_at(current_instruction & 0x1F);
}

void CPU::execute() {
    unsigned int data = present_instruction & 0x1F;

    switch(present_instruction & 0xE000) {
        case 0x0: // JMP
            current_instruction = store.line_at(data);
            break;
        case 0x2000: // JRP
            current_instruction += store.line_at(data);
            break;
        case 0x4000: // LDN
            accumulator = (-1) * store.line_at(data);
            break;
        case 0x6000: // STO
            store.set_data(data, accumulator);
            break;
        case 0x8000: // SUB
            accumulator -= store.line_at(data);
            break;
        case 0xC000: // CMP
            if(accumulator < 0) {
                current_instruction++;
            }
            break;
        case 0xE000: // STP
            cpu_state = HALT;
            return;
        default:
            std::cout << "\nError unknown instruction\n";
            cpu_state = HALT;
            return;
    }

    cpu_state = FETCH;
}

void CPU::run() {

    // Check if the cpu initialization was correct
    if(cpu_state != HALT) {
        std::cout << "\nWrong CPU state\n";
        return;
    }

    cpu_state = FETCH;

    print_control();
    while(cpu_state != HALT) {

        switch (cpu_state) {
            case FETCH:
                fetch();
                cpu_state = EXECUTE;
                break;
            case EXECUTE:
                execute();
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

unsigned int CPU::get_ac() {
    return accumulator;
}

unsigned int CPU::get_ci() {
    return current_instruction;
}

unsigned int CPU::get_pi() {
    return present_instruction;
}
