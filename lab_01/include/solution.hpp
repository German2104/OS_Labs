#pragma once

#include <vector>
#include <string>

// Функция для вычисления среднего арифметического
float calculateAverage(const std::vector<float>& numbers);

// Функция для чтения чисел из файла
bool readFloatsFromFile(const std::string& filename, std::vector<float>& numbers);

// Функция для создания дочернего процесса
pid_t createFork();