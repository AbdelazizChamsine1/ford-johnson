#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

class BitcoinExchange {
private:
    std::map<std::string, double> _database;
    
    bool isValidDate(const std::string& date);
    bool isValidValue(const std::string& value);
    double stringToDouble(const std::string& str);
    std::string trim(const std::string& str);
    std::string findClosestDate(const std::string& date);
    
public:
    BitcoinExchange();
    ~BitcoinExchange();
    
    bool loadDatabase(const std::string& filename);
    void processInput(const std::string& filename);
};

#endif