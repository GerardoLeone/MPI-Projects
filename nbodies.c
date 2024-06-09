#include <mpi.h> 
#include <stdio.h> 
#include <string.h> 

int main(int argc, char** argv) {
    int msg = 15;
    char string[20] = "";
    int arrayBcast[10];
    int arrayScattering[10] = {0};
    int recvArrayScattering[2] = {0};  // Buffer per ricevere i dati scatter

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int arrayGather[100];

    int gatherValue = world_rank * 100;
    
    if(world_rank == 0) {
        MPI_Send(&msg, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        printf("Int Sent: %d\n", msg);
        
        scanf("%s", string);
        MPI_Send(string, strlen(string)+1, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
        printf("String Sent: %s\n", string);

        //--------------------- BROADCAST
        for(int i = 0; i < 10; i++) {
            arrayBcast[i] = i + 1;
        }

        // Invia in bcast
        MPI_Bcast(arrayBcast, 10, MPI_INT, 0, MPI_COMM_WORLD);

        //--------------------- SCATTER
        // Inizializza arrayScattering solo nel rank 0
        for(int i = 0; i < 10; i++) {
            arrayScattering[i] = i + 1;
        }

        MPI_Scatter(arrayScattering, 2, MPI_INT, recvArrayScattering, 2, MPI_INT, 0, MPI_COMM_WORLD);

        //Gather
        MPI_Gather(&gatherValue, 1, MPI_INT, arrayGather, 1, MPI_INT, 0, MPI_COMM_WORLD);
        for(int i = 0; i < world_size; i++) {
            printf("Gather Recv %d: %d\n", i, arrayGather[i]);
        }
    } else {
        if(world_rank == 1) {
            MPI_Recv(&msg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Int Received: %d\n", msg);
            
            MPI_Recv(string, 20, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("String Received: %s\n", string);
        }

        // Riceve il broadcast
        MPI_Bcast(arrayBcast, 10, MPI_INT, 0, MPI_COMM_WORLD);
        
        printf("Bcast array received by %d: ", world_rank);
        for(int i = 0; i < 10; i++)
            printf("%d, ", arrayBcast[i]);
        printf("\n");

        // Riceve lo scatter
        MPI_Scatter(NULL, 0, MPI_INT, recvArrayScattering, 2, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Scattered data received by %d: %d, %d\n", world_rank, recvArrayScattering[0], recvArrayScattering[1]);

        //Gather
        MPI_Gather(&gatherValue, 1, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Gather Send %d: %d\n", world_rank, gatherValue);
    }

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}
