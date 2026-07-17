#include "day02.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstddef>


void GiftShop::readPuzzleInput() {

    std::ifstream file(puzzleInput);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    Interval i;
    char dash;
    
    while (file >> i.start >> dash >> i.end) {
        ranges.push_back(i);
        
        while (file.peek() == ',' || file.peek() == ' ' || file.peek() == '\n' || file.peek() == '\r')
            file.get();
    }
    file.close();
}


bool Interval::isValidID(long long current) const {

    // convert current ID to string
    std::string str = std::to_string(current);
    size_t n = str.length();

    // if leading zero: invalid ID
    if (n > 0 && str[0] == '0')
        return false;

    // if length is odd: valid ID    
    if (n % 2 != 0)
        return true;

    // if length is even and both halves match: invalid ID
    size_t halfLen = n / 2;
    if (str.substr(0,halfLen) == str.substr(halfLen, halfLen))
        return false;    
    
    // otherwise, valid ID
    return true;
}


long long Interval::sumInvalidIDs() const {

    long long sum = 0;

    for (long long s = start; s <= end; ++s) {
        if (!isValidID(s)) {
            sum += s;
        }
    }
    
    return sum;
}

bool Interval::isValidIDPart2(long long current) const {

    // same as part 1, convert current ID to string
    std::string str = std::to_string(current);
    size_t n = str.length();

    // same initial checks as part 1, if leading zero: invalid ID
    if (n > 0 && str[0] == '0')
        return false;

    size_t maxLen = n / 2;    
    for (size_t len = 1; len <= maxLen; ++len) {
        if (n % len == 0) {
            std::string base = str.substr(0,len);
            std::string rebuilt = "";
            size_t repetitions = n / len;
            for (size_t r = 0; r < repetitions; ++r) {
                rebuilt = rebuilt + base;
            }

            if (rebuilt == str) {
                return false;
            }
        }
    }

    // otherwise
    return true;

}


long long Interval::sumInvalidIDsPart2() const {

    long long sum = 0;
    for (long long s = start; s <= end; ++s) {
        if (!isValidIDPart2(s)) {
            sum += s;
        }
    }
    return sum;
}

long long GiftShop::checkInterval(size_t i) const {    
    return ranges[i].sumInvalidIDs();
}


long long GiftShop::solvePart1(bool comment) const {
    
    long long sum = 0;
    for (size_t i = 0; i < ranges.size(); ++i) {
        sum += checkInterval(i);
    }
    if (comment) std::cout << "Total Sum (Part 1 Solution) = " << sum << std::endl;
    return sum;
}



long long GiftShop::checkIntervalPart2(size_t i) const {    
    return ranges[i].sumInvalidIDsPart2();
}

long long GiftShop::solvePart2(bool comment) const {
    
    long long sum = 0;
    for (size_t i = 0; i < ranges.size(); ++i) {
        sum += checkIntervalPart2(i);
    }
    if (comment) std::cout << "Total Sum (Part 2 Solution) = " << sum << std::endl;
    return sum;
}




