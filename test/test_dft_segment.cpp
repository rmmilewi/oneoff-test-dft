#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include "../generated-code.hpp"
#include "include/test_framework.hpp"
#include "include/test_utilities.hpp"

void testDftSegment() {
    test::TestSuite suite("DFT Segment Computation Tests");
    
    const double TOLERANCE = 1e-10;
    
    // Test 1: Simple input [1, 0, 0, 0]
    // Expected output: [1, 1, 1, 1] (all ones)
    {
        std::vector<double> input = {1, 0, 0, 0};
        int N = input.size();
        
        // Compute full DFT using our segment function
        std::vector<std::complex<double>> result = computeDftSegment(input, 0, N, N);
        
        // Expected result: all ones
        std::vector<std::complex<double>> expected(N, std::complex<double>(1, 0));
        
        suite.assertComplexVectorEqual(expected, result, TOLERANCE, 
                                      "DFT of [1, 0, 0, 0] should be all ones");
    }
    
    // Test 2: Input [1, 1, 1, 1]
    // Expected output: [4, 0, 0, 0] (first element is sum, others are zero)
    {
        std::vector<double> input = {1, 1, 1, 1};
        int N = input.size();
        
        // Compute full DFT using our segment function
        std::vector<std::complex<double>> result = computeDftSegment(input, 0, N, N);
        
        // Expected result: [4, 0, 0, 0]
        std::vector<std::complex<double>> expected = {
            std::complex<double>(4, 0),
            std::complex<double>(0, 0),
            std::complex<double>(0, 0),
            std::complex<double>(0, 0)
        };
        
        suite.assertComplexVectorEqual(expected, result, TOLERANCE, 
                                      "DFT of [1, 1, 1, 1] should be [4, 0, 0, 0]");
    }
    
    // Test 3: Compare with baseline implementation for [1, 2, 3, 4]
    {
        std::vector<double> input = {1, 2, 3, 4};
        int N = input.size();
        
        // Compute using our segment function
        std::vector<std::complex<double>> result = computeDftSegment(input, 0, N, N);
        
        // Compute using baseline implementation
        std::vector<std::complex<double>> expected;
        testCorrectDft(input, expected);
        
        // Our testCorrectDft now uses the same sign convention as our implementation
        
        suite.assertComplexVectorEqual(expected, result, TOLERANCE, 
                                      "DFT of [1, 2, 3, 4] should match baseline implementation");
    }
    
    // Test 4: Test partial segment computation
    {
        std::vector<double> input = {1, 2, 3, 4};
        int N = input.size();
        
        // Compute only the first half of the DFT
        std::vector<std::complex<double>> result = computeDftSegment(input, 0, N/2, N);
        
        // Compute full DFT using baseline implementation
        std::vector<std::complex<double>> fullExpected;
        testCorrectDft(input, fullExpected);
        
        // Extract first half
        std::vector<std::complex<double>> expected;
        for (int k = 0; k < N/2; ++k) {
            expected.push_back(fullExpected[k]);
        }
        
        suite.assertComplexVectorEqual(expected, result, TOLERANCE, 
                                      "Partial DFT segment should match corresponding part of full DFT");
    }
}

int main() {
    testDftSegment();
    return 0;
}