#include "PmergeMe.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./pmerge <numbers separated by space or as separate arguments>" << std::endl;
        return 1;
    }

    std::vector<int> vector_data;
    std::deque<int> deque_data;

    //// vector ////
    std::clock_t start_vector = std::clock();
    PmergeSort::run(argv, vector_data);
    std::clock_t end_vector = std::clock();
    double duration_vector = (end_vector - start_vector) / (double)CLOCKS_PER_SEC ;

    //// deque ////
    std::clock_t start_deque = std::clock();
    PmergeSort::run(argv, deque_data);
    std::clock_t end_deque = std::clock();
    double duration_deque = (end_deque - start_deque) / (double)CLOCKS_PER_SEC ;

    std::cout << "before:         ";
    for (int i = 1; i < argc; ++i)
        std::cout << argv[i] << (i + 1 < argc ? " " : "\n");
    
    std::cout << "after:          ";
    for (std::size_t i = 0; i < vector_data.size(); ++i)
        std::cout << vector_data[i] << (i + 1 < vector_data.size() ? " " : "\n");

    std::cout << "Time to process a range of " << vector_data.size()
            << " elements with std::vector : "
            << std::fixed << std::setprecision(5) << duration_vector << " us" << std::endl;

    std::cout << "Time to process a range of " << deque_data.size()
              << " elements with std::deque : "
              << std::fixed << std::setprecision(5) << duration_deque << " us" << std::endl;
    
    std::cout << "comparisons:    " << count_vector << std::endl;

    return 0;
}
