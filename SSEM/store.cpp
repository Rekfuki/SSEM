#include <iostream>
#include "store.h"

Store::Store() = default;
Store::~Store() = default;

void Store::process_data(std::vector<std::string> data) {
    for(auto &line: data){
        std::reverse(line.begin(), line.end());

        store.push_back(str_to_uint32(line));
    }
}

std::vector<unsigned int> Store::get_data() {
    return store;
}

unsigned int Store::line_at(unsigned int i) {
    return store.at(i);
}

void Store::set_data(unsigned int i, unsigned int data) {
    store.at(i) = data;
}

unsigned int Store::str_to_uint32(const std::string &line) {
    auto uint = static_cast<uint32_t>(std::stoul(line, nullptr, 2));
    return uint;
}

