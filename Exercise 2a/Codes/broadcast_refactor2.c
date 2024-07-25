#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>
#include <math.h>
#define USE MPI

//NOTE: The differences between "broadcast_refactor1.c" and this are commented in CAPSLOCK

//////////////////////////////////////////////// DEFINE THE PROCESS STRUCTURE ////////////////////////////////////////////////////////////////////////////////////////

//Element of tree of processes
typedef struct{
	int ID; //process ID
	int left; //The position of the left leaf on the tree array
	int right; //The position of the right leaf on the tree array
	int parent; //The position of the parent node on the tree array
}Process;

////////////////////////////////////////////// PROCESSES BINARY TREE FUNCTIONS /////////////////////////////////////////////////////////////////////////////

//This function initialize a standard process tree feature given N processes in the tree
//TO IMPROVE THE ACCESSES THE COMPLETION OF LEAFS AND PARENTS ARE MADE PREVIOUSLY
//AND NOT IN ORDER. IN THIS WAY ALL THE OPERATIONS FOR THE i PROCESS ARE MADE IN ONE TIME
//AT THE END THE NUMBER OF IFs IS THE SAME!
void initializeProcessTree(Process* InputTree,int N){
	for(int i=0;i<N;i++){
		InputTree[i].ID = i; //Set ID

		if((2*i+2)<N){  //Check if it has a right leaf (and so the left) and set the links
			InputTree[i].left = 2*i+1;
			InputTree[i].right = 2*i+2;
		}
		else{ //No right
			InputTree[i].right = -1;
			if((2*i+1)<N){ //So check at least left and set
				InputTree[i].left = 2*i+1;
			}
			else{
				InputTree[i].left = -1;
			}
		}

		InputTree[i].parent = floor((i-1)/2); //The parent is set numerically thanks to the structure
	}
	return;
}

//This function switch the node in the position P with the root adjusting all the links
void switchRoot(Process* InputTree,int P){
	if(P==0){return;} //P==0 no change

	unsigned short int tempID = InputTree[P].ID;
	InputTree[P].ID = InputTree[0].ID; //Set P ID
	//P leaves' parent set
	if(InputTree[P].left!=-1) InputTree[2*P+1].parent=InputTree[P].ID;
	if(InputTree[P].right!=-1) InputTree[2*P+2].parent=InputTree[P].ID;

	//P as leaf
	if(P % 2 == 0) InputTree[(P-2)/2].right = InputTree[P].ID;
	else InputTree[(P-1)/2].left = InputTree[P].ID;

	//Then set the 0 properties
	InputTree[0].ID = tempID;
	//Root leaves's parent
	InputTree[1].parent = tempID;
	InputTree[2].parent = tempID;
}


//A simple check DEBUG function used to control if the tree is correct. I leave here for you to check if necessary
void printTree(Process* InputTree,int NP){
	printf("THE TREE:\n");
	for(int i=0;i<NP;i++){
		printf("%d#%d#%d-%d ",InputTree[i].parent,InputTree[i].ID,InputTree[i].left,InputTree[i].right);
		if(i % 2 == 0) printf("\n");
	}
	printf("\n");
}

/////////////////////////////////////////////////////////// THE BROADCASTING ALGORITHM ////////////////////////////////////////////////////////

void TreeBroadcast(void* buffer,int count,MPI_Datatype datatype,int root,MPI_Comm comm){

	//MPI Setups

	int numproc,myid;
	int tag=123;

	MPI_Comm_size(comm,&numproc);
  	MPI_Comm_rank(comm,&myid);

  	MPI_Status status;

  	//Create the tree of processes

  	Process* ProcessTree = malloc(numproc*sizeof *ProcessTree);
	initializeProcessTree(ProcessTree,numproc);
  	switchRoot(ProcessTree,root); //Change the root with the one of the starting process of the broadcast (default ID= 0)

  	//Performed the task for each one of the node following the tree scheme
  	if((myid!= root)&&(myid!=0)){
  		MPI_Recv(buffer,count,datatype,ProcessTree[myid].parent,tag,comm,&status);
  		if(ProcessTree[myid].left!=-1) MPI_Send(buffer,count,datatype,ProcessTree[myid].left,tag,comm);
  		if(ProcessTree[myid].right!=-1) MPI_Send(buffer,count,datatype,ProcessTree[myid].right,tag,comm);
  	}
  	else{
	  	if(myid==root){ //works also if root==0
	  		MPI_Send(buffer,count,datatype,ProcessTree[0].left,tag,comm);
	  		MPI_Send(buffer,count,datatype,ProcessTree[0].right,tag,comm);
	  	}
	  	else{ //myid == 0
	  		MPI_Recv(buffer,count,datatype,ProcessTree[root].parent,tag,comm,&status);
	  		if(ProcessTree[root].left!=-1) MPI_Send(buffer,count,datatype,ProcessTree[root].left,tag,comm);
	  		if(ProcessTree[root].right!=-1) MPI_Send(buffer,count,datatype,ProcessTree[root].right,tag,comm);
	  	}
  	}

  	MPI_Barrier(comm);

	free(ProcessTree);
}

/////////////////////////////////////////////////// TESTING PART ////////////////////////////////////////////////////////////////////////

void initializeProcessTree(Process* InputTree,int N);
void switchRoot(Process* InputTree,int P);
void TreeBroadcast(void* buffer,int count,MPI_Datatype datatype,int root,MPI_Comm comm);

int main(int argc, char *argv[])
{
	double start_time,end_time; //time variable to test the process time
	int numproc,myid;
	int buff_size = 5; //size of the buffer
	int root = 1; //The root of the broadcast

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numproc);
  	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	int buffer[5]={0,0,0,0,0};

  	if(myid==root){ //Define the test buffer for the root
  		buffer[0]=1;
  		buffer[1]=2;
  		buffer[2]=3;
  		buffer[3]=4;
  		buffer[4]=5;
  	}

	MPI_Barrier(MPI_COMM_WORLD); //Prepare all the nodes and start the Broadcast
  	start_time = MPI_Wtime();

	TreeBroadcast(&buffer,buff_size,MPI_INT,root,MPI_COMM_WORLD);

	end_time = MPI_Wtime(); //End of broadcast

	if(myid==0){ //Computation time
		printf("COMPUTATION TIME: %10.6f\n",end_time-start_time);
	}

	MPI_Finalize();

	return 0;
}


