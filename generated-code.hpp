#include <vector>
#include <complex>
#include <mpi.h>
#include <cmath>
#include <tuple>

/**
 * Calculate the work distribution for a given process in MPI.
 * 
 * @param N Total size of the problem
 * @param size Total number of MPI processes
 * @param rank Current process rank
 * @return A tuple containing start_k and end_k indices for this process
 */
std::tuple<int, int> calculateWorkDistribution(int N, int size, int rank) {
    int local_N = N / size;
    int remainder = N % size;
    
    // Compute the range of output indices for this process
    int start_k = rank * local_N + std::min(rank, remainder);
    int end_k = start_k + local_N + (rank < remainder ? 1 : 0);
    
    return std::make_tuple(start_k, end_k);
}

/**
 * Compute a segment of the DFT for the given range of k values.
 * This is a pure function with no MPI dependencies.
 * 
 * @param x Input signal
 * @param start_k Starting k index (inclusive)
 * @param end_k Ending k index (exclusive)
 * @param N Size of the input signal
 * @return Vector of complex values for the computed DFT segment
 */
std::vector<std::complex<double>> computeDftSegment(
    const std::vector<double>& x, 
    int start_k, 
    int end_k, 
    int N
) {
    std::vector<std::complex<double>> segment_output(end_k - start_k);
    const double PI = std::acos(-1);
    
    for (int k = start_k; k < end_k; ++k) {
        std::complex<double> sum(0.0, 0.0);
        for (int n = 0; n < N; ++n) {
            double angle = -2 * PI * k * n / N;
            sum += x[n] * std::exp(std::complex<double>(0, angle));
        }
        segment_output[k - start_k] = sum;
    }
    
    return segment_output;
}

/**
 * Set up the counts and displacements arrays for MPI_Gatherv.
 * 
 * @param N Total size of the problem
 * @param size Total number of MPI processes
 * @return A tuple containing the counts and displacements vectors
 */
std::tuple<std::vector<int>, std::vector<int>> setupGatherOperation(int N, int size) {
    int remainder = N % size;
    std::vector<int> counts(size);
    std::vector<int> displs(size);
    
    for (int i = 0; i < size; ++i) {
        counts[i] = N / size + (i < remainder ? 1 : 0);
        displs[i] = (i == 0) ? 0 : displs[i - 1] + counts[i - 1];
    }
    
    return std::make_tuple(counts, displs);
}

/**
 * Gather results from all processes to the root process.
 * 
 * @param local_output Local output from this process
 * @param N Total size of the problem
 * @param rank Current process rank
 * @param counts Vector of counts for each process
 * @param displs Vector of displacements for each process
 * @return Full output vector (only valid on rank 0)
 */
std::vector<std::complex<double>> gatherResults(
    const std::vector<std::complex<double>>& local_output,
    int N,
    int rank,
    const std::vector<int>& counts,
    const std::vector<int>& displs
) {
    std::vector<std::complex<double>> full_output;
    if (rank == 0) {
        full_output.resize(N);
    }
    
    MPI_Gatherv(
        local_output.data(), local_output.size(), MPI_DOUBLE_COMPLEX,
        full_output.data(), counts.data(), displs.data(), MPI_DOUBLE_COMPLEX,
        0, MPI_COMM_WORLD
    );
    
    return full_output;
}

/**
 * Compute the discrete Fourier transform of x using MPI parallelization.
 * 
 * @param x Input signal
 * @param output Output vector to store the DFT result
 */
void dft(std::vector<double> const& x, std::vector<std::complex<double>> &output) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = x.size();
    
    // Calculate work distribution for this process
    auto [start_k, end_k] = calculateWorkDistribution(N, size, rank);
    
    // Compute the assigned portion of the DFT
    std::vector<std::complex<double>> local_output = 
        computeDftSegment(x, start_k, end_k, N);
    
    // Set up for gathering results
    auto [counts, displs] = setupGatherOperation(N, size);
    
    // Gather results from all processes
    std::vector<std::complex<double>> full_output = 
        gatherResults(local_output, N, rank, counts, displs);
    
    // Assign the result to the output parameter (only on rank 0)
    if (rank == 0) {
        output = std::move(full_output);
    }
}
