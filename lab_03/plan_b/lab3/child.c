#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <semaphore.h>

#include "SystemErrors/errors.h"

const char SHARED_MEM_NAME[] = "/shared_memory_example";
const int SHARED_MEM_SIZE = 4096;

const char SEM_NAME[] = "/my_semaphore";

int CharToDouble(const char *string, double *result) {
    int k = -1;
    int fl = 0;
    double number = 0;
    for (int j = 0; string[j] != '\0'; ++j) {
        if (string[j] == '-' && fl == 0) {
            fl = 1;
        } else if (string[j] >= '0' && string[j] <= '9') {
            number *= 10;
            number += (string[j] - '0');
            if (number < 0) {
                return 2;
            }
            if (k != -1) k += 1;
        } else if (string[j] == '.' && k == -1) {
            k = 0;
        } else if (string[j] == '\n') {
            break;
        } else {
            return 1;
        }
    }
    if (k == 0) return 0;
    for (int k_null = 0; k_null < k; ++k_null) number /= 10.0;
    if (fl) number *= -1;
    *result = number;
    return 0;
}

int main() {
    int shm_fd = shm_open(SHARED_MEM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        return print_error((error_msg) {INPUT_FILE_ERROR, "main", "shm fd didn't open"});
    }

    double *shared_mem = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED) {
        shm_unlink(SHARED_MEM_NAME);
        return print_error((error_msg) {INPUT_FILE_ERROR, "main", "mmap didn't work"});
    }

    sem_t *sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        shm_unlink(SHARED_MEM_NAME);
        munmap(shared_mem, SHARED_MEM_SIZE);
        return print_error((error_msg) {INPUT_FILE_ERROR, "main", "semaphore didn't create"});
    }

    int k = 0;
    double sum = 0;
    char input[4096];
    char c;
    char buffer[1024];
    int i = 0;
    int j;
    ssize_t n;
    double num = 0;

    while ((n = read(STDIN_FILENO, input, sizeof(input))) > 0) {
        j = 0;
        while (j < n) {
            c = input[j++];
            if (c == ' ' && i != 0) {
                buffer[i] = '\0';
                if (CharToDouble(buffer, &num)) {
                    sem_post(sem);
                    munmap(shared_mem, SHARED_MEM_SIZE);
                    sem_close(sem);
                    const char msg[] = "error: unrecognized character\n";
                    write(STDERR_FILENO, msg, sizeof(msg));
                    exit(EXIT_FAILURE);
                }
                i = 0;
                sum += num;
            } else if (c == '\n') {
                buffer[i] = '\0';
                if (CharToDouble(buffer, &num)) {
                    sem_post(sem);
                    munmap(shared_mem, SHARED_MEM_SIZE);
                    sem_close(sem);
                    const char msg[] = "error: unrecognized character\n";
                    write(STDERR_FILENO, msg, sizeof(msg));
                    exit(EXIT_FAILURE);
                }
                sum += num;
                shared_mem[k++] = sum;
                sem_post(sem);
                sum = 0;
                i = 0;
            } else {
                buffer[i++] = c;
            }
        }
    }

    // Завершаем работу, если ввод завершен
    if (i != 0) {
        buffer[i] = '\0';
        if (CharToDouble(buffer, &num)) {
            sem_post(sem);
            munmap(shared_mem, SHARED_MEM_SIZE);
            sem_close(sem);
            const char msg[] = "error: unrecognized character\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(EXIT_FAILURE);
        }
        sum += num;
        shared_mem[k++] = sum;
        sem_post(sem);
    }

    // Сигнализируем о завершении
    sem_post(sem);
    sem_post(sem);
    munmap(shared_mem, SHARED_MEM_SIZE);
    sem_close(sem);
    exit(EXIT_SUCCESS);
}