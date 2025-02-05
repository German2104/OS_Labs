#include "solution.hpp"
#include <iostream>
#include <dlfcn.h>
#include <vector>
#include <sstream>

void static_link_test() {
    std::cout << "Static linking test:\n";
    
    long number = 10;
    char* bin = translation(number, 2);
    std::cout << number << " in binary: " << bin << std::endl;
    free(bin);

    int arr[] = {5, 3, 8, 1, 2};
    int* sorted = sort(arr, 5, true);
    
    std::cout << "Sorted array (quick sort): ";
    for (size_t i = 0; i < 5; ++i) std::cout << sorted[i] << " ";
    std::cout << std::endl;

    free(sorted);
}

void dynamic_link_test() {
    std::cout << "Dynamic linking test:\n";

    void* lib_handle = dlopen("./libsolution.so", RTLD_LAZY);
    if (!lib_handle) {
        std::cerr << "Error loading library: " << dlerror() << std::endl;
        return;
    }

    auto translation = (char* (*)(long, int)) dlsym(lib_handle, "translation");
    auto sort = (int* (*)(int*, size_t, bool)) dlsym(lib_handle, "sort");

    if (!translation || !sort) {
        std::cerr << "Error loading functions: " << dlerror() << std::endl;
        dlclose(lib_handle);
        return;
    }

    long number = 10;
    char* bin = translation(number, 3);
    std::cout << number << " in ternary: " << bin << std::endl;
    free(bin);

    int arr[] = {5, 3, 8, 1, 2};
    int* sorted = sort(arr, 5, false);

    std::cout << "Sorted array (bubble sort): ";
    for (size_t i = 0; i < 5; ++i) std::cout << sorted[i] << " ";
    std::cout << std::endl;

    free(sorted);
    dlclose(lib_handle);
}

int main() {
    std::string command;
    while (true) {
        std::cout << "Enter command (0 - switch, 1 x base, 2 size [arr...], q - quit): ";
        std::getline(std::cin, command);

        if (command == "q") break;

        std::istringstream ss(command);
        int cmd;
        ss >> cmd;

        if (cmd == 1) {
            long x;
            int base;
            ss >> x >> base;
            char* result = translation(x, base);
            if (result) {
                std::cout << "Result: " << result << std::endl;
                free(result);
            } else {
                std::cout << "Error: invalid base\n";
            }
        } else if (cmd == 2) {
            size_t size;
            ss >> size;
            std::vector<int> arr(size);
            for (size_t i = 0; i < size; ++i) ss >> arr[i];

            int* sorted = sort(arr.data(), size, false);
            if (sorted) {
                std::cout << "Sorted: ";
                for (size_t i = 0; i < size; ++i) std::cout << sorted[i] << " ";
                std::cout << std::endl;
                free(sorted);
            }
        } else {
            std::cout << "Invalid command\n";
        }
    }

    return 0;
}