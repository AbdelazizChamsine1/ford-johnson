#ifndef PMERGE_SORT_HPP
#define PMERGE_SORT_HPP

#include <vector>
#include <deque>
#include <set>
#include <string>
#include <sstream>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <algorithm>

extern int count_vector;

class PmergeSort {
public:
    static void run(char** av, std::vector<int>& data);
    static void run(char** av, std::deque<int>& data);

private:
    static int JacobsthalSequence(int n);

    // Vector
    static std::vector<int> recursiveSort(std::vector<int>& data);
    static void insertSort(std::vector<int>& main, std::vector<int>& pend);
    static std::vector<int> generateJacobIndices(int size);
    static void editIndices(std::vector<int>& jacob, std::vector<int> pend);
    static void insertPend(std::vector<int>& main, std::vector<int>& pend, std::vector<int>& jacob);
    static int binarySearch(std::vector<int>& main, int value, int high);

    // Deque
    static std::deque<int> recursiveSort(std::deque<int>& data);
    static void insertSort(std::deque<int>& main, std::deque<int>& pend);
    static std::deque<int> generateJacobIndicesDeque(int size);
    static void editIndices(std::deque<int>& jacob, std::deque<int> pend);
    static void insertPend(std::deque<int>& main, std::deque<int>& pend, std::deque<int>& jacob);
    static int binarySearch(std::deque<int>& main, int value, int high);
};

#endif
