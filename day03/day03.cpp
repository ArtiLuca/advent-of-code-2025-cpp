#include "day03.h"

#include <string>
#include <exception>
#include <fstream>
#include <vector>
#include <iostream>

// =============== PUZZLE INPUT ===============

void Escalator::readPuzzleInput() {

    banks.clear();

    std::ifstream file(puzzleInput);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Bank b;
        b.batteries = line;
        banks.push_back(b);
    }
    file.close();
}


// =============== PART 1 ===============

int Bank::findJoltage() const {

    int n = batteries.length();

    // guard
    if (n < 2) {
        throw std::runtime_error("Bank has fewer than 2 batteries.");
    }

    // find first maximum
    char max1 = '/'; 
    int idx1 = -1;
    // scan entire string
    for (int i = 0; i < n; ++i) {
        if (batteries[i] > max1) {
            max1 = batteries[i];
            idx1 = i;
        }
        // optimization
        if (max1 == '9') {
            break;
        }
    }

    // case 1: max1 is not last digit
    if (idx1 < n - 1) {
        char max2 = '/';
        // scan to right of max1 to find max2
        for (int j = idx1 + 1; j < n; ++j) {
            if (batteries[j] > max2)
                max2 = batteries[j];
            if (max2 == '9')
                break;    
        }
        return (max1 - '0') * 10 + (max2 - '0');
    }

    // case 2: max1 is the last digit
    else {
        char max2 = '/';
        // scan left of max1 to find max2
        for (int j = 0; j < idx1; ++j) {
            if (batteries[j] > max2)
                max2 = batteries[j];
            if (max2 == '9')
                break;    
        }
        return (max2  - '0') * 10 + (max1 - '0'); 
    }

    // otherwise
    return -1;
}

int Escalator::solvePart1() const {

    int sum = 0;
    for (const auto& bank : banks) 
        sum += bank.findJoltage();
    return sum;    
}


// =============== PART 2 ===============

long long Bank::findJoltagePart2() const {

    int n = batteries.length();
    
    int k = 12;
    
    // guard
    if (n < k) {
        throw std::runtime_error("Invalid bank of batteries.");
    }

    int discard = n - k;

    // allocate stack structure (S) to simulate the Greedy logic
    std::vector<int> S;
    S.reserve(k);

    // scan the bank string from left to right
    for(int i = 0; i < n; ++i) {

        int current = batteries[i] - '0';

        // until S contains digits, the last digit inserted is strictly smaller  than the current one, 
        // and we still have "discards" available ...
        while (!S.empty() && S.back() < current && discard > 0) {
            // ... it is locally optimal to discard the previous element
            S.pop_back(); // 
            discard = discard - 1;
        }

        // insert the current element at the top of the stack
        S.push_back(current);
    }

    // if we have discards remaining (e.g., the array was already sorted in 
    // descending order), the excess digits will be at the end, so we ignore them.
    while (discard > 0) {
            S.pop_back();
            discard--;
        }

    // we build the final number positionally using the first k elements    
    long long result = 0;
    for (int j = 0; j < k; ++j) {
        result = (result * 10) + S[j];
    }
        
    return result;
}


long long Escalator::solvePart2() const {

    long long sum = 0;
    for (const auto& bank : banks) {
        sum += bank.findJoltagePart2();
    }

    return sum;
}
