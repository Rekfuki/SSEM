//
// Created by Andrius Navasaitis on 06/11/2018.
//

#include <iostream>
#include "cpu.h"

CPU::CPU(Store &store) : accumulator(0),
present_instruction(0), cpu_state(HALT), store(store) {
    current_instruction = 0;
}

CPU::~CPU() = default;

void CPU::fetch() {
    current_instruction++;
    present_instruction = store.line_at(current_instruction & 0x1F);
}

void CPU::execute() {
    unsigned int data = present_instruction & 0x1F;

    printf("DATA: %d\n", data);
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
            std::cout << "Error unknown instruction" << std::endl;
            cpu_state = HALT;
            return;
    }

    cpu_state = FETCH;
}

void CPU::halt() {
}

void CPU::run() {

    // Check if the cpu initialization was correct
    if(cpu_state != HALT) {
        std::cout << "Wrong CPU state" << std::endl;
        return;
    }

    cpu_state = FETCH;

    while(cpu_state != HALT) {

        switch (cpu_state) {
            case FETCH:
                fetch();
                cpu_state = EXECUTE;
                break;
            case EXECUTE:
                execute();
                break;
            default:
                std::cout << "Wrong cpu state" << std::endl;
                return;
        }
    }
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
