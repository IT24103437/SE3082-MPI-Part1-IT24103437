#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    const long long total_points = 10000000;
    int rank, size;
    long long local_inside = 0;
    long long total_inside = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long start = (total_points * rank) / size;
    long long end = (total_points * (rank + 1)) / size;
    unsigned int seed = 12345U + (unsigned int)rank * 10007U;

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (long long i = start; i < end; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            local_inside++;
        }
    }

    if (rank == 0) {
        total_inside = local_inside;

        for (int i = 1; i < size; i++) {
            long long received;
            MPI_Status status;

            MPI_Recv(&received, 1, MPI_LONG_LONG_INT,
                     MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            total_inside += received;
            printf("Received count from rank %d\n", status.MPI_SOURCE);
        }
    } else {
        MPI_Send(&local_inside, 1, MPI_LONG_LONG_INT,
                 0, 0, MPI_COMM_WORLD);
    }

    double local_time = MPI_Wtime() - start_time;
    double total_time = 0.0;

    MPI_Reduce(&local_time, &total_time, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = 4.0 * total_inside / total_points;
        printf("Points = %lld\n", total_points);
        printf("Points inside = %lld\n", total_inside);
        printf("Estimated Pi = %.8f\n", pi);
        printf("Time = %.6f seconds\n", total_time);
    }

    MPI_Finalize();
    return 0;
}