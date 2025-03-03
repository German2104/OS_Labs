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
sudo strace ./main_exe ../test/tests01.txt
sudo strace ./main_exe ../test/tests02.txt
