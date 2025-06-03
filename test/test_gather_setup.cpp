#include <iostream>
#include <vector>
#include <tuple>
#include "../generated-code.hpp"
#include "include/test_framework.hpp"

void testGatherSetup() {
    test::TestSuite suite("Gather Operation Setup Tests");
    
    // Test 1: N = 10, size = 4
    // With remainder 2, ranks 0 and 1 should get 3 elements, ranks 2 and 3 should get 2 elements
    {
        auto [counts, displs] = setupGatherOperation(10, 4);
        
        std::vector<int> expectedCounts = {3, 3, 2, 2};
        std::vector<int> expectedDispls = {0, 3, 6, 8};
        
        suite.assertEqual(expectedCounts, counts, "Counts for N=10, size=4");
        suite.assertEqual(expectedDispls, displs, "Displacements for N=10, size=4");
    }
    
    // Test 2: N = 8, size = 4
    // With no remainder, all ranks should get 2 elements
    {
        auto [counts, displs] = setupGatherOperation(8, 4);
        
        std::vector<int> expectedCounts = {2, 2, 2, 2};
        std::vector<int> expectedDispls = {0, 2, 4, 6};
        
        suite.assertEqual(expectedCounts, counts, "Counts for N=8, size=4");
        suite.assertEqual(expectedDispls, displs, "Displacements for N=8, size=4");
    }
    
    // Test 3: N = 3, size = 4
    // Edge case where N < size, some ranks get 1 element, others get 0
    {
        auto [counts, displs] = setupGatherOperation(3, 4);
        
        std::vector<int> expectedCounts = {1, 1, 1, 0};
        std::vector<int> expectedDispls = {0, 1, 2, 3};
        
        suite.assertEqual(expectedCounts, counts, "Counts for N=3, size=4");
        suite.assertEqual(expectedDispls, displs, "Displacements for N=3, size=4");
    }
    
    // Test 4: N = 0, size = 4
    // Edge case with empty input
    {
        auto [counts, displs] = setupGatherOperation(0, 4);
        
        std::vector<int> expectedCounts = {0, 0, 0, 0};
        std::vector<int> expectedDispls = {0, 0, 0, 0};
        
        suite.assertEqual(expectedCounts, counts, "Counts for N=0, size=4");
        suite.assertEqual(expectedDispls, displs, "Displacements for N=0, size=4");
    }
}

int main() {
    testGatherSetup();
    return 0;
}