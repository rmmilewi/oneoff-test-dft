#include <iostream>
#include <vector>
#include <tuple>
#include "../generated-code.hpp"
#include "include/test_framework.hpp"

void testWorkDistribution() {
    test::TestSuite suite("Work Distribution Tests");
    
    // Test 1: N = 10, size = 4, various ranks
    // With remainder 2, ranks 0 and 1 should get 3 elements, ranks 2 and 3 should get 2 elements
    suite.assertTupleEqual(std::make_tuple(0, 3), calculateWorkDistribution(10, 4, 0), 
                          "Rank 0 with N=10, size=4");
    suite.assertTupleEqual(std::make_tuple(3, 6), calculateWorkDistribution(10, 4, 1), 
                          "Rank 1 with N=10, size=4");
    suite.assertTupleEqual(std::make_tuple(6, 8), calculateWorkDistribution(10, 4, 2), 
                          "Rank 2 with N=10, size=4");
    suite.assertTupleEqual(std::make_tuple(8, 10), calculateWorkDistribution(10, 4, 3), 
                          "Rank 3 with N=10, size=4");
    
    // Test 2: N = 8, size = 4, various ranks
    // With no remainder, all ranks should get 2 elements
    suite.assertTupleEqual(std::make_tuple(0, 2), calculateWorkDistribution(8, 4, 0), 
                          "Rank 0 with N=8, size=4");
    suite.assertTupleEqual(std::make_tuple(2, 4), calculateWorkDistribution(8, 4, 1), 
                          "Rank 1 with N=8, size=4");
    suite.assertTupleEqual(std::make_tuple(4, 6), calculateWorkDistribution(8, 4, 2), 
                          "Rank 2 with N=8, size=4");
    suite.assertTupleEqual(std::make_tuple(6, 8), calculateWorkDistribution(8, 4, 3), 
                          "Rank 3 with N=8, size=4");
    
    // Test 3: N = 3, size = 4, various ranks
    // Edge case where N < size, some ranks get 1 element, others get 0
    suite.assertTupleEqual(std::make_tuple(0, 1), calculateWorkDistribution(3, 4, 0), 
                          "Rank 0 with N=3, size=4");
    suite.assertTupleEqual(std::make_tuple(1, 2), calculateWorkDistribution(3, 4, 1), 
                          "Rank 1 with N=3, size=4");
    suite.assertTupleEqual(std::make_tuple(2, 3), calculateWorkDistribution(3, 4, 2), 
                          "Rank 2 with N=3, size=4");
    suite.assertTupleEqual(std::make_tuple(3, 3), calculateWorkDistribution(3, 4, 3), 
                          "Rank 3 with N=3, size=4");
}

int main() {
    testWorkDistribution();
    return 0;
}