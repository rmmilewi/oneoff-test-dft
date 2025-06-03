#include <vector>
#include <complex>
#include <mpi.h>
#include <cmath>

void dft(std::vector<double> const& x, std::vector< std::complex< double > > &output) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = x.size();
    int local_N = N / size;
    int remainder = N % size;

    // Compute the range of output indices for this process
    int start_k = rank * local_N + std::min(rank, remainder);
    int end_k = start_k + local_N + (rank < remainder ? 1 : 0);

    // Compute the assigned portion of the DFT
    std::vector<std::complex<double> > local_output(end_k - start_k);
    const double PI = std::acos(-1);
    for (int k = start_k; k < end_k; ++k) {
        std::complex<double> sum(0.0, 0.0);
        for (int n = 0; n < N; ++n) {
            double angle = -2 * PI * k * n / N;
            sum += x[n] * std::exp(std::complex<double>(0, angle));
        }
        local_output[k - start_k] = sum;
    }

    // Gather all results to root process
    std::vector<int> counts(size);
    std::vector<int> displs(size);
    for (int i = 0; i < size; ++i) {
        counts[i] = N / size + (i < remainder ? 1 : 0);
        displs[i] = (i == 0) ? 0 : displs[i - 1] + counts[i - 1];
    }

    std::vector<std::complex<double> > full_output;
    if (rank == 0)
        full_output.resize(N);

    MPI_Gatherv(local_output.data(), local_output.size(), MPI_DOUBLE_COMPLEX,
                full_output.data(), counts.data(), displs.data(), MPI_DOUBLE_COMPLEX,
                0, MPI_COMM_WORLD);

    if (rank == 0)
        output = std::move(full_output);
}
