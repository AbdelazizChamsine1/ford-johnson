#include "PmergeMe.hpp"

// int main(int argc, char** argv) {
//     if (argc < 2) {
//         std::cerr << "Usage: ./pmerge <numbers separated by space or as separate arguments>" << std::endl;
//         return 1;
//     }

//     std::vector<int> vector_data;
//     std::deque<int> deque_data;

//     std::cout << "\n--- Sorting with std::vector ---" << std::endl;
//     PmergeSort::run(argv, vector_data);
//     std::cout << "after:          ";
//     for (std::size_t i = 0; i < vector_data.size(); ++i)
//         std::cout << vector_data[i] << (i + 1 < vector_data.size() ? " " : "\n");
//     std::cout << "comparisons:    " << count_vector << std::endl;

//     std::cout << "\n--- Sorting with std::deque ---" << std::endl;
//     PmergeSort::run(argv, deque_data);
//     std::cout << "after:          ";
//     for (std::size_t i = 0; i < deque_data.size(); ++i)
//         std::cout << deque_data[i] << (i + 1 < deque_data.size() ? " " : "\n");
//     std::cout << "comparisons:    " << count_vector << std::endl;

//     return 0;
// }

#include <iostream>
#include <vector>
#include <deque>
#include <ctime>
#include <iomanip> // for std::setprecision

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./pmerge <numbers separated by space or as separate arguments>" << std::endl;
        return 1;
    }

    std::vector<int> vector_data;
    std::deque<int> deque_data;

    // --- Timing vector sort ---
    std::cout << "\n--- Sorting with std::vector ---" << std::endl;
    std::clock_t start_vector = std::clock();
    PmergeSort::run(argv, vector_data);
    std::clock_t end_vector = std::clock();
    double duration_vector = (end_vector - start_vector) / (double)CLOCKS_PER_SEC * 1000000.0;

    std::cout << "after:          ";
    for (std::size_t i = 0; i < vector_data.size(); ++i)
        std::cout << vector_data[i] << (i + 1 < vector_data.size() ? " " : "\n");
    std::cout << "comparisons:    " << count_vector << std::endl;
    std::cout << "Time to process a range of " << vector_data.size()
              << " elements with std::vector : "
              << std::fixed << std::setprecision(5) << duration_vector << " us" << std::endl;

    // --- Timing deque sort ---
    std::cout << "\n--- Sorting with std::deque ---" << std::endl;
    std::clock_t start_deque = std::clock();
    PmergeSort::run(argv, deque_data);
    std::clock_t end_deque = std::clock();
    double duration_deque = (end_deque - start_deque) / (double)CLOCKS_PER_SEC * 1000000.0;

    std::cout << "after:          ";
    for (std::size_t i = 0; i < deque_data.size(); ++i)
        std::cout << deque_data[i] << (i + 1 < deque_data.size() ? " " : "\n");
    std::cout << "comparisons:    " << count_vector << std::endl;
    std::cout << "Time to process a range of " << deque_data.size()
              << " elements with std::deque : "
              << std::fixed << std::setprecision(5) << duration_deque << " us" << std::endl;

    return 0;
}
