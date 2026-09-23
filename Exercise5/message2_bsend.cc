#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int values[3] = {0, 10, 20};

        std::vector<char> buffer(3 * (MPI_BSEND_OVERHEAD + sizeof(int)));
        MPI_Buffer_attach(buffer.data(), static_cast<int>(buffer.size()));

        for (int i = 0; i < 3; i++) {
            MPI_Bsend(&values[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent " << values[i] << "\n";
        }

        void* detached_buffer;
        int detached_size;
        MPI_Buffer_detach(&detached_buffer, &detached_size);
    } else if (rank == 1) {
        for (int i = 0; i < 3; i++) {
            int received;
            MPI_Recv(&received, 1, MPI_INT, 0, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << received << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}