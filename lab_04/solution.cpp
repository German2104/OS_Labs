#include "solution.hpp"
#include <cstring>
#include <cstdlib>

// Функция перевода числа в другую систему счисления
char* translation(long x, int base) {
    if (base != 2 && base != 3) {
        return nullptr; // Поддерживаются только 2 и 3
    }

    char buffer[65]; // 64 бит + 1 символ '\0'
    int index = 64;
    buffer[index] = '\0';

    if (x == 0) {
        buffer[--index] = '0';
    } else {
        bool negative = x < 0;
        if (negative) x = -x;

        while (x > 0) {
            buffer[--index] = '0' + (x % base);
            x /= base;
        }

        if (negative) buffer[--index] = '-';
    }

    char* result = strdup(&buffer[index]);
    return result;
}

// Пузырьковая сортировка
void bubble_sort(int* array, size_t size) {
    for (size_t i = 0; i < size - 1; ++i) {
        for (size_t j = 0; j < size - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                std::swap(array[j], array[j + 1]);
            }
        }
    }
}

// Быстрая сортировка (Хоара)
void quick_sort(int* array, int left, int right) {
    if (left >= right) return;
    int pivot = array[right];
    int i = left, j = right - 1;

    while (i <= j) {
        while (i <= j && array[i] < pivot) i++;
        while (i <= j && array[j] > pivot) j--;
        if (i <= j) std::swap(array[i++], array[j--]);
    }

    std::swap(array[i], array[right]);
    quick_sort(array, left, j);
    quick_sort(array, i + 1, right);
}

// Функция сортировки с выбором алгоритма
int* sort(int* array, size_t size, bool use_quick_sort) {
    int* sorted_array = (int*)malloc(size * sizeof(int));
    if (!sorted_array) return nullptr;
    
    std::memcpy(sorted_array, array, size * sizeof(int));
    
    if (use_quick_sort) {
        quick_sort(sorted_array, 0, size - 1);
    } else {
        bubble_sort(sorted_array, size);
    }

    return sorted_array;
}