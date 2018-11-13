#ifndef MANCHESTERBABY_STORE_H
#define MANCHESTERBABY_STORE_H


#include <vector>

class Store {
    public:
        Store();
        ~Store();

        void process_data(std::vector<std::string> data);
        std::vector<unsigned int> get_data();
        unsigned int line_at(unsigned int i);
        void set_data(unsigned int i, unsigned int data);

private:
        unsigned int str_to_uint32(const std::string &line);

        std::vector<unsigned int> store;
};


#endif //MANCHESTERBABY_STORE_H
