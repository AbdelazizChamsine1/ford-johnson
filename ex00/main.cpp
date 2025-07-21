#include "BitcoinExchange.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }
    
    BitcoinExchange btc;
    
    // Load the database (assuming data.csv exists)
    if (!btc.loadDatabase("data.csv")) {
        return 1;
    }
    
    // Process input file
    btc.processInput(argv[1]);
    
    return 0;
}