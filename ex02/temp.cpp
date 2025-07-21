#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>
#include <cstdlib>

class PmergeSort {
public:
    void run(char** input);
private:
    std::vector<int> parseInput(char** args);
    std::vector<int> recursiveSort(std::vector<int>& sequence);
    std::vector<int> generateJacobsthalIndices(size_t max);
    void refineIndices(std::vector<int>& indices, const std::vector<int>& pend);
    void insertPending(std::vector<int>& sorted, const std::vector<int>& pend, const std::vector<int>& indices);
    int findInsertPosition(const std::vector<int>& sorted, int value, int high);
    void mergeSortedWithPending(std::vector<int>& sorted, const std::vector<int>& pend);
    int jacobsthal(int n);
    void printSequence(const std::string& label, const std::vector<int>& sequence);
};

void PmergeSort::run(char** input) {
    std::vector<int> data = parseInput(input);
    printSequence("Before", data);
    std::vector<int> sorted = recursiveSort(data);
    printSequence("After", sorted);
}

std::vector<int> PmergeSort::parseInput(char** args) {
    std::vector<int> data;
    for (int i = 1; args[i]; ++i) {
        std::stringstream ss(args[i]);
        std::string token;
        while (std::getline(ss, token, ' ')) {
            if (token.empty()) continue;

            for (size_t j = 0; j < token.size(); ++j) {
                if (!isdigit(token[j]) && !(j == 0 && token[j] == '+')) {
                    throw std::invalid_argument("Invalid input: " + token);
                }
            }

            long value = std::strtol(token.c_str(), nullptr, 10);
            if (value > INT_MAX || value < 0) {
                throw std::out_of_range("Out of range: " + token);
            }
            data.push_back(static_cast<int>(value));
        }
    }
    return data;
}

std::vector<int> PmergeSort::recursiveSort(std::vector<int>& sequence) {
    if (sequence.size() <= 1) return sequence;
    if (sequence.size() == 2) {
        if (sequence[0] > sequence[1])
            std::swap(sequence[0], sequence[1]);
        return sequence;
    }

    std::vector<int> main;
    std::vector<int> pend;

    for (size_t i = 0; i + 1 < sequence.size(); i += 2) {
        if (sequence[i] > sequence[i + 1]) {
            main.push_back(sequence[i]);
            pend.push_back(sequence[i + 1]);
        } else {
            main.push_back(sequence[i + 1]);
            pend.push_back(sequence[i]);
        }
    }

    if (sequence.size() % 2 != 0)
        pend.push_back(sequence.back());

    std::vector<int> sortedMain = recursiveSort(main);
    std::vector<int> reorderedPend;

    for (int val : sortedMain) {
        for (size_t j = 0; j < main.size(); ++j) {
            if (main[j] == val) {
                reorderedPend.push_back(pend[j]);
                break;
            }
        }
    }

    if (sequence.size() % 2 != 0)
        reorderedPend.push_back(pend.back());

    mergeSortedWithPending(sortedMain, reorderedPend);
    return sortedMain;
}

void PmergeSort::mergeSortedWithPending(std::vector<int>& sorted, const std::vector<int>& pend) {
    std::vector<int> indices = generateJacobsthalIndices(pend.size());
    refineIndices(indices, pend);
    insertPending(sorted, pend, indices);
}

std::vector<int> PmergeSort::generateJacobsthalIndices(size_t max) {
    std::vector<int> indices;
    for (int i = 3; ; ++i) {
        int val = jacobsthal(i);
        if (val > static_cast<int>(max)) break;
        indices.push_back(val);
    }
    return indices;
}

void PmergeSort::refineIndices(std::vector<int>& indices, const std::vector<int>& pend) {
    std::vector<int> refined;
    std::vector<int> used;

    for (int idx : indices) {
        while (idx > 1) {
            if (std::find(used.begin(), used.end(), idx) == used.end()) {
                refined.push_back(idx);
                used.push_back(idx);
            } else {
                break;
            }
            --idx;
        }
    }

    int x = pend.size();
    while (refined.size() < pend.size()) {
        if (std::find(used.begin(), used.end(), x) == used.end()) {
            refined.push_back(x);
            used.push_back(x);
        }
        --x;
    }

    indices = refined;
}

void PmergeSort::insertPending(std::vector<int>& sorted, const std::vector<int>& pend, const std::vector<int>& indices) {
    sorted.insert(sorted.begin(), pend[0]);
    int high = 3;

    for (size_t i = 1; i < indices.size(); ++i) {
        if (indices[i] > indices[i - 1])
            high = 2 * high + 1;

        int idx = indices[i];
        if (idx <= static_cast<int>(pend.size())) {
            int pos = findInsertPosition(sorted, pend[idx - 1], high - 1);
            if (pos != -1)
                sorted.insert(sorted.begin() + pos, pend[idx - 1]);
        }
    }
}

int PmergeSort::findInsertPosition(const std::vector<int>& sorted, int value, int high) {
    int low = 0;
    high = std::min(high, static_cast<int>(sorted.size()) - 1);

    while (low <= high) {
        int mid = (low + high) / 2;
        if (sorted[mid] == value) return mid;
        if (sorted[mid] > value) high = mid - 1;
        else low = mid + 1;
    }

    return (low < static_cast<int>(sorted.size()) && value < sorted[low]) ? low : sorted.size();
}

int PmergeSort::jacobsthal(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return jacobsthal(n - 1) + 2 * jacobsthal(n - 2); // For simplicity; consider memoization for performance
}

void PmergeSort::printSequence(const std::string& label, const std::vector<int>& sequence) {
    std::cout << label << ": ";
    for (size_t i = 0; i < sequence.size(); ++i)
        std::cout << sequence[i] << (i + 1 < sequence.size() ? " " : "\n");
}
