#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>
#include <math.h>
#define USE MPI

int main(int argc, char *argv[])
{
	double start_time,end_time;
	int numproc,myid;
	int buff_size = 5;
	int root = 1; 

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numproc);
  	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	int buffer[5]={0,0,0,0,0};

  	if(myid==root){
  		buffer[0]=1;
  		buffer[1]=2;
  		buffer[2]=3;
  		buffer[3]=4;
  		buffer[4]=5;
  	}

	MPI_Barrier(MPI_COMM_WORLD);

  	start_time = MPI_Wtime();

	MPI_Bcast(&buffer,5,MPI_INT,root,MPI_COMM_WORLD); //Here we use the original broadcast!

	end_time = MPI_Wtime();

	if(myid==0){ //COMPUTATION TIME
		printf("%10.6f\n",end_time-start_time);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();

	return 0;
}


