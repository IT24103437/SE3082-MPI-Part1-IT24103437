#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    const long long N = 10000000;
    long long local_sum = 0, total_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long start = (N * rank) / size + 1;
    long long end = (N * (rank + 1)) / size;

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    for (long long i = start; i <= end; i++) {
        local_sum += i;
    }

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double local_time = MPI_Wtime() - t0;
    double elapsed;
    MPI_Reduce(&local_time, &elapsed, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Sum: %lld\nTime: %.6f seconds\n", total_sum, elapsed);
    }

    MPI_Finalize();
    return 0;
}