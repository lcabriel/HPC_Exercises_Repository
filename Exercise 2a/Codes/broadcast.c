#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>
#define USE MPI

//////////////////////////////////////////////// DEFINE THE PROCESS STRUCTURE ////////////////////////////////////////////////////////////////////////////////////////

//Element of tree of processes
typedef struct{
	unsigned short int ID; //process ID
	short int left; //The position of the left leaf on the tree array
	short int right; //The position of the right leaf on the tree array
	short int parent; //The position of the parent node on the tree array
}Process;

////////////////////////////////////////////// PROCESSES BINARY TREE FUNCTIONS /////////////////////////////////////////////////////////////////////////////

//This function initialize a standard process tree feature given N processes in the tree
void initializeProcessTree(Process* InputTree,int N){
	for(int i=0;i<N;i++){
		InputTree[i].ID = i; //At start each node has the ID equal to position

		//Check if it has a left leaf and set the links
		if((2*i+1)<N){ 
			InputTree[i].left = 2*i+1;
			InputTree[2*i+1].parent=i;
		}
		else InputTree[i].left = -1; //Otherwise keep it void
		
		//Check if it has a right leaf and set the links
		if((2*i+2)<N){
			InputTree[i].right = 2*i+2;
			InputTree[2*i+2].parent=i;
		}
		else InputTree[i].right = -1; //Otherwise keep it void

	}
	InputTree[0].parent = -1; //The root has no parent
	return;
}

//This function switch the node in the position P with the root adjusting all the links
void switchRoot(Process* InputTree,int P){
	//Change ID
	unsigned short int tempID = InputTree[P].ID;
	InputTree[P].ID = InputTree[0].ID;
	InputTree[0].ID = tempID;

	//Root leaves's parent ID is changed
	InputTree[1].parent = tempID;
	InputTree[2].parent = tempID;

	//P leaves' parent set checking if they exist
	if(InputTree[P].left!=-1) InputTree[2*P+1].parent=InputTree[P].ID;
	if(InputTree[P].right!=-1) InputTree[2*P+2].parent=InputTree[P].ID;

	//P as leaf. We need to understand if the position of P is a leaf of something
	if(P % 2 == 0) InputTree[(P-2)/2].right = InputTree[P].ID;
	else InputTree[(P-1)/2].left = InputTree[P].ID;
}

//A simple check DEBUG function used to control if the tree is correct. I leave here for you to check if necessary
void printTree(Process* InputTree,int NP){
	printf("THE TREE:\n");
	for(int i=0;i<NP;i++){
		printf("%d#%d#%d-%d ",InputTree[i].parent,InputTree[i].ID,InputTree[i].left,InputTree[i].right);
		if(i % 2 == 0) printf("\n");
	}
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

	Process* ProcessTree;
	ProcessTree = malloc(numproc*sizeof *ProcessTree);
	initializeProcessTree(ProcessTree,numproc);
  	switchRoot(ProcessTree,root); //Change the root with the one of the starting process of the broadcast (default ID= 0)

  	//NOTE: I LEFT SOME DEBUFF IN THIS NON OPTIMIZE VERSION. LOOK THE NUMBER 3 FOR THE FINAL ONE.
  	//The message passing process along the tree. NOTE: some problematic logic. Need improvments
  	if(myid==root){
  		printTree(ProcessTree,numproc);
		printf("STARTBUFF:%d\n",*(int*)buffer);
  		MPI_Send(buffer,count,datatype,ProcessTree[0].left,tag,comm);
  		MPI_Send(buffer,count,datatype,ProcessTree[0].right,tag,comm);
  	}
  	if(myid==0){
  		MPI_Recv(buffer,count,datatype,ProcessTree[root].parent,tag,comm,&status);
		printf("PROC %d, BUFF: %d\n",myid,*(int*)buffer);
  		if(ProcessTree[root].left!=-1) MPI_Send(buffer,count,datatype,ProcessTree[root].left,tag,comm);
  		if(ProcessTree[root].right!=-1) MPI_Send(buffer,count,datatype,ProcessTree[root].right,tag,comm);
  	}
  	if((myid!= root)&&(myid!=0)){
  		MPI_Recv(buffer,count,datatype,ProcessTree[myid].parent,tag,comm,&status);
		printf("PROC %d, BUFF: %d\n",myid,*(int*)buffer);
  		if(ProcessTree[myid].left!=-1) MPI_Send(buffer,count,datatype,ProcessTree[myid].left,tag,comm);
  		if(ProcessTree[myid].right!=-1) MPI_Send(buffer,count,datatype,ProcessTree[myid].right,tag,comm);
  	}

  	MPI_Barrier(comm); //I placed here a barrier to grant that all the broadcasting process is completed

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

  	//Test message
	printf("Hi, i'm process %d. The buffer is {%d,%d,%d,%d,%d}\n",myid,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);

	MPI_Barrier(MPI_COMM_WORLD); //Prepare all the nodes and start the Broadcast
  	start_time = MPI_Wtime();

	TreeBroadcast(&buffer,buff_size,MPI_INT,root,MPI_COMM_WORLD);

	end_time = MPI_Wtime(); //End of broadcast

	//Debug message 
	printf("Hi, i'm process %d. The buffer is {%d,%d,%d,%d,%d}\n",myid,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);

	if(myid==0){ //and computation time
		printf("COMPUTATION TIME: %10.6f\n",end_time-start_time);
	}

	MPI_Finalize();

	return 0;
}


