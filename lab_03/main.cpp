#include "./include/solution.hpp"
#include <iostream>
#include <vector>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

#define SHM_NAME "/shared_memory"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    std::vector<float> numbers;

    if (!readFloatsFromFile(filename, numbers)) {
        return 1;
    }

    size_t size = numbers.size() * sizeof(float);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Error: Cannot create shared memory" << std::endl;
        return 1;
    }

    if (ftruncate(shm_fd, size) == -1) {
        std::cerr << "Error: Cannot resize shared memory" << std::endl;
        return 1;
    }

    void* shared_memory = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Error: Memory mapping failed" << std::endl;
        return 1;
    }

    memcpy(shared_memory, numbers.data(), size);

    pid_t pid = createFork();

    if (pid == 0) {
        // Дочерний процесс
        float* shared_numbers = static_cast<float*>(shared_memory);
        std::vector<float> childNumbers(shared_numbers, shared_numbers + numbers.size());

        float average = calculateAverage(childNumbers);
        std::cout << "Child process: Average = " << average << std::endl;

        munmap(shared_memory, size);
        close(shm_fd);
        shm_unlink(SHM_NAME);
    } else if (pid > 0) {
        // Родительский процесс
        wait(nullptr);
        munmap(shared_memory, size);
        close(shm_fd);
        shm_unlink(SHM_NAME);
    }

    return 0;
}
