#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <iostream>

extern "C" {
    char* translation(long x, int base);
    int* sort(int* array, size_t size, bool use_quick_sort);
}

#endif // SOLUTION_HPP