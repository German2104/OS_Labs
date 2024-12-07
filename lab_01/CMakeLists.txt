cmake_minimum_required(VERSION 3.10)
project(ForkedAverageCalculator)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Добавляем библиотеку для решения
add_library(Solution src/solution.cpp)

# Включаем директорию с заголовочными файлами
target_include_directories(Solution PUBLIC include)

# Создание основного исполняемого файла
add_executable(main_exe main.cpp)

# Линковка библиотеки с исполняемым файлом
target_link_libraries(main_exe PRIVATE Solution)
