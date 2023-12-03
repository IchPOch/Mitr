#include <iostream>
#include <mpi.h>


int div_up(int x, int y)
{
    return (x - 1) / y + 1;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int num_students = 10;
    int ticket[num_students];
    bool has_ticket = false;
    int students[num_students];

    if (rank == 0) {
         /*Generating tickets for all students*/
        for (int i = 0; i < num_students; ++i) {
            ticket[i] = rand() % (100 - 1 + 1) + 1;  // Random ticket numbers for demonstration
            students[i] = i+1;

        }

        /* Sending tickets to all students*/
        for (int i = 1; i < size; ++i) MPI_Send(&ticket[i], 1, MPI_INT, rank, i, MPI_COMM_WORLD);
        
    }
    else {
        has_ticket = true;

        MPI_Status status;
        int count;
        /*MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);*/

        MPI_Get_count(&status, MPI_INT, &count);
        int ticketfor;
        int countThreadforProces = size - 1;
        int countStudentperThread = div_up(num_students, countThreadforProces);

        for (int i = (rank - 1) * countStudentperThread; i < countStudentperThread + (rank - 1) * countStudentperThread - 1; ++i) {
            MPI_Recv(&ticketfor, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Send(&students[i], 1, MPI_INT,rank,i, MPI_COMM_WORLD);
            
            
            std::cout << ticketfor << std::endl;

        }
        int PassStudent;
        
        for (int i = (rank - 1) * countStudentperThread; i < countStudentperThread + (rank - 1) * countStudentperThread - 1; ++i) {
            MPI_Recv(&PassStudent, 1, MPI_INT, rank, i, MPI_COMM_WORLD, &status);
            std::cout << "Conductor: Student " << PassStudent << " ticket: " << ticket[PassStudent] << ", you can enter.\n";
        }
    }
        MPI_Finalize();
    return 0;
}