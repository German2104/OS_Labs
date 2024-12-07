#include "./include/solution.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    std::vector<float> numbers;

    // Читаем числа из файла
    if (!readFloatsFromFile(filename, numbers)) {
        return 1; // Ошибка при чтении файла
    }

    // Создаем канал (pipe)
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Error: Cannot create pipe" << std::endl;
        return 1;
    }

    // Используем функцию createFork для создания дочернего процесса
    pid_t pid = createFork();

    if (pid == 0) {
        // === Дочерний процесс ===
        close(pipefd[1]); // Закрываем канал для записи

        // Чтение данных из канала
        size_t size;
        read(pipefd[0], &size, sizeof(size)); // Считываем размер вектора

        std::vector<float> childNumbers(size);
        read(pipefd[0], childNumbers.data(), size * sizeof(float)); // Считываем числа

        close(pipefd[0]); // Закрываем канал для чтения

        // Вычисляем среднее арифметическое
        float average = calculateAverage(childNumbers);
        std::cout << "Child process: Average = " << average << std::endl;

    } else if (pid > 0) {
        // === Родительский процесс ===
        close(pipefd[0]); // Закрываем канал для чтения

        // Отправка данных в канал
        size_t size = numbers.size();
        write(pipefd[1], &size, sizeof(size)); // Отправляем размер вектора
        write(pipefd[1], numbers.data(), size * sizeof(float)); // Отправляем числа

        close(pipefd[1]); // Закрываем канал для записи

        // Ожидаем завершения дочернего процесса
        wait(nullptr);
    }

    return 0;
}