#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
	int my_rank, p;
	MPI_Comm comm;

	MPI_Init(&argc, &argv);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &p);
	MPI_Comm_rank(comm, &my_rank);

	if£¨my_rank == 0£©{
		cout << "I'm 0" << endl;
	}
	else {
	cout << "i'm" << my_rank << endl;
	}
}