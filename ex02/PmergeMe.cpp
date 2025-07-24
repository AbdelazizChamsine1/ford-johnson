#include "PmergeMe.hpp"
#include <set>

int count_vector = 0;

int PmergeSort::JacobsthalSequence(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    int prev2 = 0, prev1 = 1, curr = 0;
    for (int i = 2; i <= n; ++i) {
        curr = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

/////////////////////////////// vector /////////////////////////////////////////////

void checkDuplicates(const std::vector<int>& data) {
    std::set<int> seen;

    for (std::size_t i = 0; i < data.size(); ++i) {
        if (seen.find(data[i]) != seen.end()) {
            std::cerr << "Error: Duplicate number found => " << data[i] << std::endl;
            exit(1);
        }
        seen.insert(data[i]);
    }
}

std::vector<int> PmergeSort::recursiveSort(std::vector<int>& data) {
    std::vector<int> result;
    if (data.empty()) return result;
    if (data.size() == 1) return data;
    if (data.size() == 2) {
        result = data;
        count_vector++;
        if (result[0] > result[1])
            std::swap(result[0], result[1]);
        return result;
    }

    bool isOdd = data.size() % 2 == 1;
    std::vector<int> main, pend;
    for (std::size_t i = 0; i + 1 < data.size(); i += 2) {
        count_vector++;
        if (data[i] > data[i + 1]) {
            main.push_back(data[i]);
            pend.push_back(data[i + 1]);
        } else {
            pend.push_back(data[i]);
            main.push_back(data[i + 1]);
        }
    }
    if (isOdd)
        pend.push_back(data.back());

    std::vector<int> new_main = recursiveSort(main);
    std::vector<int> new_pend(new_main.size());

    for (std::size_t i = 0; i < new_main.size(); ++i) {
        for (std::size_t j = 0; j < main.size(); ++j) {
            if (main[j] == new_main[i]) {
                new_pend[i] = pend[j];
                break;
            }
        }
    }
    if (isOdd)
        new_pend.push_back(pend.back());

    insertSort(new_main, new_pend);
    return new_main;
}

void PmergeSort::run(char** av, std::vector<int>& data) {
    for (int i = 1; av[i]; ++i) {
        std::string arg = av[i];
        std::stringstream ss(arg);
        std::string number;

        while (std::getline(ss, number, ' ')) {
            if (!number.empty())
            {
                for (std::size_t j = 0; j < number.size(); ++j) {
                    if (!std::isdigit(number[j]) && !(j == 0 && number[j] == '+')) {
                        std::cerr << "Error: Invalid input => " << number << std::endl;
                        exit(1);
                    }
                }

                char* end;
                long value = std::strtol(number.c_str(), &end, 10);
                if (value > INT_MAX || value < 0) {
                    std::cerr << "Error: Out of range => " << number << " must be between 0 and 2147483647 " << std::endl;
                    exit(1);
                }
                data.push_back(static_cast<int>(value));
            }
        }
    }
    checkDuplicates(data);
    count_vector = 0;
    data = recursiveSort(data);
}

// int PmergeSort::JacobsthalSequence(int n) {
//     if (n == 0) return 0;
//     if (n == 1) return 1;
//     int prev2 = 0, prev1 = 1, curr = 0;
//     for (int i = 2; i <= n; ++i) {
//         curr = prev1 + 2 * prev2;
//         prev2 = prev1;
//         prev1 = curr;
//     }
//     return curr;
// }
// std::vector<int> PmergeSort::generateJacobIndices(int size) {
//     std::vector<int> jacob;
//     int index = 3;
//     while (JacobsthalSequence(index) <= size) {
//         jacob.push_back(JacobsthalSequence(index));
//         ++index;
//     }
//     return jacob;
// }

std::vector<int> PmergeSort::generateJacobIndices(int size) {
    std::vector<int> jacob;
    int prev2 = 0, prev1 = 1, curr = 0;
    int index = 2;

    // Start from index = 3, so precompute the first two
    while (true) {
        curr = prev1 + 2 * prev2;
        if (curr > size)
            break;
        jacob.push_back(curr);
        prev2 = prev1;
        prev1 = curr;
        ++index;
    }

    return jacob;
}


void PmergeSort::editIndices(std::vector<int>& jacob, std::vector<int> pend) {
    std::vector<int> temp, pushed;
    for (std::size_t i = 0; i < jacob.size(); ++i) {
        int x = jacob[i];
        while (x > 1) {
            if (std::find(pushed.begin(), pushed.end(), x) == pushed.end()) {
                temp.push_back(x);
                pushed.push_back(x);
            } else {
                break;
            }
            --x;
        }
    }

    if (temp.size() == pend.size()) {
        jacob = temp;
    } else {
        int x = pend.size();
        while (temp.size() < pend.size()) {
            if (std::find(pushed.begin(), pushed.end(), x) == pushed.end()) {
                temp.push_back(x);
                pushed.push_back(x);
            } else {
                break;
            }
            --x;
        }
        jacob = temp;
    }
}

void PmergeSort::insertPend(std::vector<int>& main, std::vector<int>& pend, std::vector<int>& jacob) {
    int high = 3;
    main.insert(main.begin(), pend[0]);

    for (std::size_t i = 0; i < jacob.size(); ++i) {
        if (i > 0 && jacob[i] > jacob[i - 1])
            high = 2 * high + 1;
        if (jacob[i] <= static_cast<int>(pend.size()) && jacob[i] != 1) {
            int idx = binarySearch(main, pend[jacob[i] - 1], high - 1);
            if (idx != -1)
                main.insert(main.begin() + idx, pend[jacob[i] - 1]);
        }
    }
}

int PmergeSort::binarySearch(std::vector<int>& main, int value, int high) {
    if (main.empty())
        return 0;

    int low = 0;
    if (high >= static_cast<int>(main.size()))
        high = static_cast<int>(main.size()) - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        count_vector++;
        if (main[mid] == value)
            return mid;
        else if (main[mid] > value)
            high = mid - 1;
        else
            low = mid + 1;
    }

    return (low < static_cast<int>(main.size()) && value < main[low]) ? low : main.size();
}

void PmergeSort::insertSort(std::vector<int>& main, std::vector<int>& pend) {
    std::vector<int> jacob = generateJacobIndices(pend.size());
    editIndices(jacob, pend);
    insertPend(main, pend, jacob);
}

////////////////////////////// DEQUE /////////////////////////////////////////////////

void checkDuplicates(const std::deque<int>& data) {
    std::set<int> seen;

    for (std::size_t i = 0; i < data.size(); ++i) {
        if (seen.find(data[i]) != seen.end()) {
            std::cerr << "Error: Duplicate number found => " << data[i] << std::endl;
            exit(1);
        }
        seen.insert(data[i]);
    }
}

std::deque<int> PmergeSort::recursiveSort(std::deque<int>& data) {
    std::deque<int> result;
    if (data.empty()) return result;
    if (data.size() == 1) return data;
    if (data.size() == 2) {
        result = data;
        count_vector++;
        if (result[0] > result[1])
            std::swap(result[0], result[1]);
        return result;
    }

    bool isOdd = data.size() % 2 == 1;
    std::deque<int> main, pend;
    for (std::size_t i = 0; i + 1 < data.size(); i += 2) {
        count_vector++;
        if (data[i] > data[i + 1]) {
            main.push_back(data[i]);
            pend.push_back(data[i + 1]);
        } else {
            pend.push_back(data[i]);
            main.push_back(data[i + 1]);
        }
    }
    if (isOdd)
        pend.push_back(data.back());

    std::deque<int> new_main = recursiveSort(main);
    std::deque<int> new_pend(new_main.size());

    for (std::size_t i = 0; i < new_main.size(); ++i) {
        for (std::size_t j = 0; j < main.size(); ++j) {
            if (main[j] == new_main[i]) {
                new_pend[i] = pend[j];
                break;
            }
        }
    }
    if (isOdd)
        new_pend.push_back(pend.back());

    insertSort(new_main, new_pend);
    return new_main;
}

void PmergeSort::run(char** av, std::deque<int>& data) {
    for (int i = 1; av[i]; ++i) {
        std::string arg = av[i];
        std::stringstream ss(arg);
        std::string number;

        while (std::getline(ss, number, ' ')) {
            if (!number.empty()) {
                for (std::size_t j = 0; j < number.size(); ++j) {
                    if (!std::isdigit(number[j]) && !(j == 0 && number[j] == '+')) {
                        std::cerr << "Error: Invalid input => " << number << std::endl;
                        exit(1);
                    }
                }

                char* end;
                long value = std::strtol(number.c_str(), &end, 10);
                if (value > INT_MAX || value < 0) {
                    std::cerr << "Error: Out of range => " << number
                              << " must be between 0 and 2147483647" << std::endl;
                    exit(1);
                }

                data.push_back(static_cast<int>(value));
            }
        }
    }
    checkDuplicates(data);
    count_vector = 0;
    data = recursiveSort(data);
}

// std::deque<int> PmergeSort::generateJacobIndicesDeque(int size) {
//     std::deque<int> jacob;
//     int index = 3;
//     while (JacobsthalSequence(index) <= size) {
//         jacob.push_back(JacobsthalSequence(index));
//         ++index;
//     }
//     return jacob;
// }

std::deque<int> PmergeSort::generateJacobIndicesDeque(int size) {
    std::deque<int> jacob;
    int prev2 = 0, prev1 = 1, curr = 0;
    int index = 2;

    // Start from index = 3, so precompute the first two
    while (true) {
        curr = prev1 + 2 * prev2;
        if (curr > size)
            break;
        jacob.push_back(curr);
        prev2 = prev1;
        prev1 = curr;
        ++index;
    }

    return jacob;
}

void PmergeSort::editIndices(std::deque<int>& jacob, std::deque<int> pend) {
    std::deque<int> temp, pushed;
    for (std::size_t i = 0; i < jacob.size(); ++i) {
        int x = jacob[i];
        while (x > 1) {
            if (std::find(pushed.begin(), pushed.end(), x) == pushed.end()) {
                temp.push_back(x);
                pushed.push_back(x);
            } else {
                break;
            }
            --x;
        }
    }

    if (temp.size() == pend.size()) {
        jacob = temp;
    } else {
        int x = pend.size();
        while (temp.size() < pend.size()) {
            if (std::find(pushed.begin(), pushed.end(), x) == pushed.end()) {
                temp.push_back(x);
                pushed.push_back(x);
            } else {
                break;
            }
            --x;
        }
        jacob = temp;
    }
}

void PmergeSort::insertPend(std::deque<int>& main, std::deque<int>& pend, std::deque<int>& jacob) {
    int high = 3;
    main.insert(main.begin(), pend[0]);

    for (std::size_t i = 0; i < jacob.size(); ++i) {
        if (i > 0 && jacob[i] > jacob[i - 1])
            high = 2 * high + 1;
        if (jacob[i] <= static_cast<int>(pend.size()) && jacob[i] != 1) {
            int idx = binarySearch(main, pend[jacob[i] - 1], high - 1);
            if (idx != -1)
                main.insert(main.begin() + idx, pend[jacob[i] - 1]);
        }
    }
}

int PmergeSort::binarySearch(std::deque<int>& main, int value, int high) {
    if (main.empty())
        return 0;

    int low = 0;
    if (high >= static_cast<int>(main.size()))
        high = static_cast<int>(main.size()) - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        count_vector++;
        if (main[mid] == value)
            return mid;
        else if (main[mid] > value)
            high = mid - 1;
        else
            low = mid + 1;
    }

    return (low < static_cast<int>(main.size()) && value < main[low]) ? low : main.size();
}

void PmergeSort::insertSort(std::deque<int>& main, std::deque<int>& pend) {
    std::deque<int> jacob = generateJacobIndicesDeque(pend.size());
    editIndices(jacob, pend);
    insertPend(main, pend, jacob);
}
