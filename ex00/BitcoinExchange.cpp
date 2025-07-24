#include "BitcoinExchange.hpp"
#include <limits>
#include <cstdlib>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \n\r\t");
    size_t end = str.find_last_not_of(" \n\r\t");

    if (start == std::string::npos || end == std::string::npos)
        return "";

    return str.substr(start, end - start + 1);
}

bool BitcoinExchange::isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    
    std::string yearStr = date.substr(0, 4);
    std::string monthStr = date.substr(5, 2);
    std::string dayStr = date.substr(8, 2);

    for (int i = 0; i < 4; i++) {
        if (!std::isdigit(yearStr[i])) return false;
    }
    for (int i = 0; i < 2; i++) {
        if (!std::isdigit(monthStr[i]) || !std::isdigit(dayStr[i])) return false;
    }

    int year = atoi(yearStr.c_str());
    int month = atoi(monthStr.c_str());
    int day = atoi(dayStr.c_str());

    if (year < 0 || month < 1 || month > 12 || day < 1) return false;

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Leap year adjustment
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeap && month == 2) {
        return day <= 29;
    }

    return day <= daysInMonth[month - 1];
}


bool BitcoinExchange::isValidValue(const std::string& value) {
    if (value.empty()) return false;
    
    bool hasDecimal = false;
    size_t start = 0;
    
    // Check for negative sign
    if (value[0] == '-') return false;
    if (value[0] == '+') start = 1;
    
    // Must have at least one digit
    bool hasDigit = false;
    
    for (size_t i = start; i < value.length(); i++) {
        if (value[i] == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        } else if (value[i] >= '0' && value[i] <= '9') {
            hasDigit = true;
        } else {
            return false;
        }
    }
    
    return hasDigit;
}

double BitcoinExchange::stringToDouble(const std::string& str) {
    std::stringstream ss(str);
    double result;
    ss >> result;
    return result;
}

std::string BitcoinExchange::findClosestDate(const std::string& date) {
    std::map<std::string, double>::iterator it = _database.upper_bound(date);
    if (it == _database.begin()) {
        return _database.begin()->first;
    }
    --it;
    return it->first;
}

bool BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open database file." << std::endl;
        return false;
    }
    
    std::string line;
    bool firstLine = true;
    
    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos) continue;
        
        std::string date = trim(line.substr(0, commaPos));
        std::string valueStr = trim(line.substr(commaPos + 1));
        
        if (isValidDate(date)) {
            double value = stringToDouble(valueStr);
            _database[date] = value;
        }
    }
    
    file.close();
    return true;
}

void BitcoinExchange::processInput(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }
    
    std::string line;
    bool firstLine = true;
    
    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        
        size_t pipePos = line.find(" | ");
        if (pipePos == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        std::string date = trim(line.substr(0, pipePos));
        std::string valueStr = trim(line.substr(pipePos + 3)); // +3 for " | "

        if (!isValidDate(date)) {
            std::cerr << "Error: invalid date => " << date << std::endl;
            continue;
        }

        if (!isValidValue(valueStr)) {
            std::cerr << "Error: invalid value => " << line << std::endl;
            continue;
        }
        
        double value = stringToDouble(valueStr);

        if (value > 1000) {
            std::cerr << "Error: too large a number." <<  "=> " << value << std::endl;
            continue;
        }

        std::string closestDate = findClosestDate(date);
        double exchangeRate = _database[closestDate];
        double result = value * exchangeRate;
        
        std::cout << date << " => " << value << " = " << result << std::endl;
    }
    
    file.close();
}