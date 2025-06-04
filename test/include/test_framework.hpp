#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <cmath>
#include <complex>
#include <sstream>

// Helper function to print vectors
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i < vec.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

namespace test {

class TestSuite {
private:
    std::string name;
    int passed = 0;
    int failed = 0;
    
public:
    TestSuite(const std::string& name) : name(name) {
        std::cout << "=== Running Test Suite: " << name << " ===" << std::endl;
    }
    
    ~TestSuite() {
        std::cout << "=== Test Suite: " << name << " Complete ===" << std::endl;
        std::cout << "Passed: " << passed << ", Failed: " << failed << std::endl;
        std::cout << std::endl;
    }
    
    template<typename T>
    void assertEqual(const T& expected, const T& actual, const std::string& message) {
        if (expected == actual) {
            std::cout << "✓ PASS: " << message << std::endl;
            passed++;
        } else {
            std::cout << "✗ FAIL: " << message << std::endl;
            std::cout << "  Expected: " << expected << ", Actual: " << actual << std::endl;
            failed++;
        }
    }
    
    template<typename T>
    void assertNotEqual(const T& unexpected, const T& actual, const std::string& message) {
        if (unexpected != actual) {
            std::cout << "✓ PASS: " << message << std::endl;
            passed++;
        } else {
            std::cout << "✗ FAIL: " << message << std::endl;
            std::cout << "  Unexpected equal value: " << actual << std::endl;
            failed++;
        }
    }
    
    void assertTrue(bool condition, const std::string& message) {
        if (condition) {
            std::cout << "✓ PASS: " << message << std::endl;
            passed++;
        } else {
            std::cout << "✗ FAIL: " << message << std::endl;
            failed++;
        }
    }
    
    void assertFalse(bool condition, const std::string& message) {
        assertTrue(!condition, message);
    }
    
    // Special comparison for complex numbers with tolerance
    void assertComplexEqual(const std::complex<double>& expected, 
                           const std::complex<double>& actual, 
                           double tolerance,
                           const std::string& message) {
        bool equal = std::abs(expected.real() - actual.real()) <= tolerance &&
                     std::abs(expected.imag() - actual.imag()) <= tolerance;
        
        if (equal) {
            std::cout << "✓ PASS: " << message << std::endl;
            passed++;
        } else {
            std::cout << "✗ FAIL: " << message << std::endl;
            std::cout << "  Expected: " << expected << ", Actual: " << actual << std::endl;
            failed++;
        }
    }
    
    // Special comparison for vectors of complex numbers with tolerance
    void assertComplexVectorEqual(const std::vector<std::complex<double>>& expected, 
                                 const std::vector<std::complex<double>>& actual, 
                                 double tolerance,
                                 const std::string& message) {
        if (expected.size() != actual.size()) {
            std::cout << "✗ FAIL: " << message << " (size mismatch)" << std::endl;
            std::cout << "  Expected size: " << expected.size() << ", Actual size: " << actual.size() << std::endl;
            failed++;
            return;
        }
        
        bool allEqual = true;
        for (size_t i = 0; i < expected.size(); ++i) {
            if (std::abs(expected[i].real() - actual[i].real()) > tolerance ||
                std::abs(expected[i].imag() - actual[i].imag()) > tolerance) {
                allEqual = false;
                break;
            }
        }
        
        if (allEqual) {
            std::cout << "✓ PASS: " << message << std::endl;
            passed++;
        } else {
            std::cout << "✗ FAIL: " << message << std::endl;
            std::cout << "  Vectors differ by more than tolerance " << tolerance << std::endl;
            for (size_t i = 0; i < std::min(expected.size(), size_t(3)); ++i) {
                std::cout << "  Index " << i << ": Expected " << expected[i] 
                          << ", Actual " << actual[i] << std::endl;
            }
            if (expected.size() > 3) {
                std::cout << "  (showing only first 3 elements)" << std::endl;
            }
            failed++;
        }
    }
    
    // Special comparison for tuples of integers
    void assertTupleEqual(const std::tuple<int, int>& expected, 
                         const std::tuple<int, int>& actual, 
                         const std::string& message) {
        if (expected == actual) {
            std::cout << "✓ PASS: " << message << std::endl;
            passed++;
        } else {
            std::cout << "✗ FAIL: " << message << std::endl;
            std::cout << "  Expected: (" << std::get<0>(expected) << ", " << std::get<1>(expected) << ")" << std::endl;
            std::cout << "  Actual: (" << std::get<0>(actual) << ", " << std::get<1>(actual) << ")" << std::endl;
            failed++;
        }
    }
    
    int getPassCount() const { return passed; }
    int getFailCount() const { return failed; }
};

} // namespace test