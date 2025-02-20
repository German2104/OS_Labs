#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;

// Самодельный семафор для ограничения количества одновременно работающих потоков
class Semaphore {
private:
    mutex mtx;
    condition_variable cv;
    int count;
public:
    Semaphore(int count_) : count(count_) {}

    void wait() {
        unique_lock<mutex> lock(mtx);
        while(count <= 0) {
            cv.wait(lock);
        }
        count--;
    }

    void notify() {
        unique_lock<mutex> lock(mtx);
        count++;
        cv.notify_one();
    }
};

// Глобальный указатель на семафор, используется только на верхнем уровне вычислений
Semaphore* gSemaphore = nullptr;

// Функция для получения минора матрицы
vector<vector<int>> getMinor(const vector<vector<int>>& matrix, int row, int col) {
    int size = matrix.size();
    vector<vector<int>> minor(size - 1, vector<int>(size - 1));
    for (int i = 1, mi = 0; i < size; i++, mi++) {
        for (int j = 0, mj = 0; j < size; j++) {
            if (j == col)
                continue;
            minor[mi][mj++] = matrix[i][j];
        }
    }
    return minor;
}

// Функция для вычисления детерминанта методом разложения по первой строке.
// Если useThreads == true, то на верхнем уровне запускаются потоки, при этом
// их число ограничивается семафором.
int determinant(const vector<vector<int>>& matrix, bool useThreads) {
    int size = matrix.size();
    if (size == 1)
        return matrix[0][0];
    if (size == 2)
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

    int det = 0;
    vector<thread> threads;
    vector<int> results(size, 0);

    for (int col = 0; col < size; col++) {
        int sign = (col % 2 == 0) ? 1 : -1;
        if (useThreads) {
            // Перед запуском нового потока ждем, пока не освободится слот
            gSemaphore->wait();
            threads.emplace_back([&, col, sign]() {
                vector<vector<int>> minor = getMinor(matrix, 0, col);
                // На рекурсивных уровнях не используем многопоточность
                results[col] = sign * matrix[0][col] * determinant(minor, false);
                // Освобождаем слот в семафоре после завершения задачи
                gSemaphore->notify();
            });
        } else {
            vector<vector<int>> minor = getMinor(matrix, 0, col);
            results[col] = sign * matrix[0][col] * determinant(minor, false);
        }
    }

    if (useThreads) {
        for (auto& th : threads) {
            th.join();
        }
    }

    for (int val : results) {
        det += val;
    }
    return det;
}

// Функция генерации случайной матрицы размером size x size
void generateMatrix(vector<vector<int>>& matrix, int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(-10, 10);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = dist(gen);
        }
    }
}

int main() {
    int size;
    int maxThreads;
    cout << "Введите размер квадратной матрицы: ";
    cin >> size;
    cout << "Введите максимальное количество потоков: ";
    cin >> maxThreads;

    // Создаем глобальный семафор с заданным количеством потоков
    Semaphore sem(maxThreads);
    gSemaphore = &sem;

    vector<vector<int>> matrix(size, vector<int>(size));
    generateMatrix(matrix, size);

    cout << "\nМатрица:" << "\n";
    for (const auto& row : matrix) {
        for (int val : row)
            cout << val << " ";
        cout << "\n";
    }

    auto start = chrono::high_resolution_clock::now();
    int det = determinant(matrix, true);
    auto end = chrono::high_resolution_clock::now();

    cout << "\nОпределитель: " << det << "\n";
    cout << "Время вычисления: "
         << chrono::duration<double>(end - start).count() << " сек\n";

    return 0;
}
