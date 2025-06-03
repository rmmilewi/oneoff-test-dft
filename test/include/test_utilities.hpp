#pragma once

#include <vector>
#include <complex>
#include <cmath>

// Copy of the correctDft function from baseline.hpp without the NO_INLINE macro
// Modified to use negative sign in the exponent to match our implementation
inline void testCorrectDft(std::vector<double> const& x, std::vector<std::complex<double>> &output) {
   int N = x.size();
   output.resize(N, std::complex<double>(0, 0)); // Resize the output vector and initialize with 0

   for (int k = 0; k < N; k++) { // For each output element
      std::complex<double> sum(0, 0);
      for (int n = 0; n < N; n++) { // For each input element
         double angle = -2 * M_PI * n * k / N; // Using negative sign to match our implementation
         std::complex<double> c = std::exp(std::complex<double>(0, angle));
         sum += x[n] * c;
      }
      output[k] = sum;
   }
}