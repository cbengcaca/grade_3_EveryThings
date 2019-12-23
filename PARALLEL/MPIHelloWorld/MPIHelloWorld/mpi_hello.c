/* File:       
 *    mpi_hello.c
 *
 * Purpose:    
 *    A "hello,world" program that uses MPI
 *
 * Compile:    
 *    mpicc -g -Wall -std=C99 -o mpi_hello mpi_hello.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_hello
 *
 * Input:      
 *    None
 * Output:     
 *    A greeting from each process
 *
 * Algorithm:  
 *    Each process sends a message to process 0, which prints 
 *    the messages it has received, as well as its own message.
 *
 * IPP:  Section 3.1 (pp. 84 and ff.)
 */
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */ 
#include <math.h>
#include <time.h>
#include <stdlib.h>
const int MAX_STRING = 100;

int main(int argc, char*argv[]) {
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */
   
   /* Start up MPI */
   MPI_Init(&argc, &argv); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
   
   int pointNum = 0;
   int totalNum;
   int pointIn = 0;
   int pointTotal;
   if (my_rank == 0) { 
      /* Create message */
	   scanf_s("%d",&totalNum);
	   pointNum = totalNum / (comm_sz-1);
	   printf_s("total is %d,p is %d\n",totalNum,comm_sz-1);
     
   } 
   MPI_Bcast(&pointNum, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Barrier( MPI_COMM_WORLD );
   srand((unsigned)time(0) + (unsigned)my_rank);
   
   if(my_rank != 0) {  
	   printf_s("rank%d in caculate\n",my_rank);
	   srand((unsigned)time(0) + (unsigned)my_rank);
	   for (int i = 0; i < pointNum; i++)
	   {
		   //srand((unsigned)time(0) + (unsigned)my_rank);
		   int x = rand()%10000 +1 ;
		   //srand((unsigned)time(0) + (unsigned)my_rank);
		   int y = rand()%10001;
		   if (my_rank == 1) {
			   //printf("rank%d,x is %d, y is %d\n", my_rank, x, y);
		   }
		   if ((x*x + y*y)<=100000000 ) {
			   pointIn++;
		   }
	   }
	   printf_s("rank%d total is %d,pointIn is %d\n",my_rank,pointNum,pointIn);
   }
   MPI_Barrier(MPI_COMM_WORLD);

   MPI_Reduce(&pointIn,&pointTotal,1,MPI_INT,MPI_SUM, 0 , MPI_COMM_WORLD);
   if (my_rank == 0) {
	   printf("pi is %lf",4 * (float)pointTotal/totalNum);
   }
   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
