#include <iostream>
#include "store.h"

// Store constructor
Store::Store() = default;

// Store destructor
Store::~Store() = default;

// Loads data to the store
void Store::process_data(std::vector<std::string> data) {
    for(auto &line: data){
        std::reverse(line.begin(), line.end());

        store.push_back(str_to_uint32(line));
    }
}

// Retrieves data from the store
std::vector<unsigned int> Store::get_data() {
    return store;
}

// Returns specified line from the store
unsigned int Store::line_at(unsigned int i) {
    return store.at(i);
}

// Sets data at the specific line
void Store::set_data(unsigned int i, unsigned int data) {
    store.at(i) = data;
}

// Converts string to unsigned int
unsigned int Store::str_to_uint32(const std::string &line) {
    auto uint = static_cast<uint32_t>(std::stoul(line, nullptr, 2));
    return uint;
}

