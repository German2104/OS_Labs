#!/bin/bash

# Удаляем старые файлы
rm -rf build
mkdir build
cd build

# Собираем всё
cmake ..
make

echo "=== Running with STATIC linking ==="
./program_static <<EOF
1 10 3
2 4 9 7 5 2
q
EOF

echo "=== Running with DYNAMIC linking ==="
LD_LIBRARY_PATH=. ./program_dynamic <<EOF
1 10 3
2 4 9 7 5 2
q
EOF