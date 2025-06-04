cmake_minimum_required(VERSION 3.10)
project(DFT_Tests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find MPI package
find_package(MPI REQUIRED)

# Include directories
include_directories(${MPI_INCLUDE_PATH})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/test/include)

# Add test executables
add_executable(test_work_distribution test_work_distribution.cpp)
add_executable(test_dft_segment test_dft_segment.cpp)
add_executable(test_gather_setup test_gather_setup.cpp)

# Link MPI libraries
target_link_libraries(test_work_distribution ${MPI_CXX_LIBRARIES})
target_link_libraries(test_dft_segment ${MPI_CXX_LIBRARIES})
target_link_libraries(test_gather_setup ${MPI_CXX_LIBRARIES})

# Add a custom target to run all tests
add_custom_target(run_tests
    COMMAND test_work_distribution
    COMMAND test_dft_segment
    COMMAND test_gather_setup
    DEPENDS test_work_distribution test_dft_segment test_gather_setup
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all tests"
)