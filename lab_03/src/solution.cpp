#include "../include/solution.hpp"
#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

// Реализация функции для вычисления среднего арифметического
float calculateAverage(const std::vector<float>& numbers) {
    if (numbers.empty()) return 0.0f;
    float sum = 0.0f;
    for (float num : numbers) {
        sum += num;
    }
    return sum / numbers.size();
}

// Реализация функции для чтения чисел из файла
bool readFloatsFromFile(const std::string& filename, std::vector<float>& numbers) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }

    float value;
    while (file >> value) {
        numbers.push_back(value);
    }

    if (file.bad()) {
        std::cerr << "Error: Reading error occurred" << std::endl;
        return false;
    }

    file.close();
    return true;
}

// Функция создания дочернего процесса
pid_t createFork() {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Error: Cannot fork process" << std::endl;
    }
    return pid;
}
