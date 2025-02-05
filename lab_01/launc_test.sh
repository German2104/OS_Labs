#!/bin/zsh

# build_process
echo "Building process"
rm -rf build
mkdir build
cd build
cmake ..
make

# run_test
echo "Running test"
./main_exe ../test/tests01.txt
./main_exe ../test/tests02.txt
