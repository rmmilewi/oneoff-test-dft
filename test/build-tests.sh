#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure and build
cmake ..
make

# Run the tests
echo "Running tests..."
./test_work_distribution
./test_dft_segment
./test_gather_setup