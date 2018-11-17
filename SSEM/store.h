#ifndef MANCHESTERBABY_STORE_H
#define MANCHESTERBABY_STORE_H


#include <vector>

class Store {
    public:
        Store();    // Store constructor
        ~Store();   // Store destructor

        void process_data(std::vector<std::string> data); // Load data into the store
        std::vector<unsigned int> get_data();             // Returns data from the store
        unsigned int line_at(unsigned int i);             // Returns specific line from the store
        void set_data(unsigned int i, unsigned int data); // Sets data to the specified line

private:
        unsigned int str_to_uint32(const std::string &line); // Converts string to unsigned int

        std::vector<unsigned int> store;                     // Store vector
};


#endif //MANCHESTERBABY_STORE_H
