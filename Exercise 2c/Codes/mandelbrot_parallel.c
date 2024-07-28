#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif

//DOES NOT WORK ON WINDOWS/VISUAL CODE
/*
#if !defined(_OPENMP)
#error "OpenMP support needed for this code"
#endif
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#define FILE_NAME "output.csv" //The name of the file where to put the results
#define NUM_THREADS 16

//NOTE: The number of threads is define as constant for test simplicity having a fixed set of argument due to the problem requests

//######################################### PREDECLARATION #########################################################
//For tidyness of the code I predeclare here the functions used in the main. The definitions of those are right below the main.

void argumentCheck(int argc,char* argv[]);
unsigned int MandelbrotSetTest(float x, float y,unsigned int N);

//######################################## MAIN ######################################################################

int main(int argc,char** argv){

    //Whole code exectution time start
    clock_t BIGSTART = clock();

    //Firstly we need to check if the arguments are meaningful
	argumentCheck(argc,argv);

    //SAVE ARGUMENTS IN VARS
    unsigned int n_x = atoi(argv[1]); //number of pixel on x axis
    unsigned int n_y = atoi(argv[2]); //number of pixel on y axis
    float x_L = atof(argv[3]); //x of the lower left corner of the domain
    float y_L = atof(argv[4]); //y of the lower left corner of the domain
    float x_R = atof(argv[5]); //x of the upper right corner of the domain
    float y_R = atof(argv[6]); //y of the upper right corner of the domain
    unsigned int Imax = atoi(argv[7]); //Max number of iterations of Mandelbrot series

    float delta_x = (x_R-x_L)*1.0/(n_x+1); //lenght of each interval along x
    float delta_y = (y_R-y_L)*1.0/(n_y+1); //lenght of each interval along y

    //Now, we will compute for each pixel the number of iterations of the Mandelbrot series in the central point.
    //Being a matrix it's only necessary to compute the position of the centers along x and y and then combine.

    //ARRAY INSTANTIATION
    float* pixelPosX = (float*)malloc(n_x*sizeof(float));
    float* pixelPosY = (float*)malloc(n_y*sizeof(float));

    //The matrix for each pixel that contain the number of iterations of the Mandelbrot series.
    unsigned int** valueMat = (unsigned int**)malloc(n_y*sizeof(unsigned int*));

    //SETUP THE PIXEL POS AND COMPLETE THE DEFINITION OF VALUE_MAT
    //NOTE: taking the centers implies to consider an half interval shift in the positions.
    for(unsigned int i=0;i<n_x;i++){
        pixelPosX[i] = x_L+(i+0.5)*delta_x;
    }
    for(unsigned int j=0;j<n_y;j++){
        pixelPosY[j] = y_L+(j+0.5)*delta_y;
        valueMat[j] = (unsigned int*)malloc(n_x*sizeof(unsigned int));
    }

    //We prepare also the file were to save the results.
    FILE *file = NULL; //NOTE: This preassignment is used then to check if "fopen" has worked correctly.
    file = fopen(FILE_NAME,"w");

    //THE ALGORITHM:
    //In this first naive parallel approach we will simple take the number of rows (n_y) and split them up
    //between the threads. E.g if we have 1000 rows we will give to the ID=0 thread the rows from 0 to 249
    //and so on. Obviously, some adjustment are made in case of not perfectly dividable n_y. We will not save
    //The starting and end point of each thread because the starting point of one domain is the end point of
    //the previous one. We need only to add an extra point for the starting 0
    //Anyway we will implement a generic version where the domain can also start and end to a specific element
    //of a row.

    //In thi array we will save the starting and the end row of each thread domain.
    int** limits;

    //###################################### START OF THE PARALLEL REGION ##########################################################
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        //####################### PREPROCESSING #################################################
        //At first we need to allocate the memory of "limits" and set the first limit (the one in (0,0))
        #pragma omp single
        {
            limits = (int**)malloc((NUM_THREADS+1)*sizeof(int*));
            //Set the first limit point
            limits[0] = (int*)malloc(2*sizeof(int)); //allocation
            limits[0][0] = 0; //The 0's x starting position
            limits[0][1] = 0; //The 0's y starting position
        }

        //Save the ID of each thread
        unsigned int ID = omp_get_thread_num();

        //Now each thread can compute its own upper limit of the domain.
        //The x will be 0 because we are only dividing per rows. The last thread has the burden
        //of the remains.
        if(ID!=NUM_THREADS-1){
            limits[ID+1] = (int*)malloc(2*sizeof(int)); //allocation
            limits[ID+1][0] = 0; //The ID's x upper limit
            limits[ID+1][1] = roundf((ID+1)*n_y*1.0/NUM_THREADS); //The ID's y upper limit
        }
        else{
            limits[ID+1] = (int*)malloc(2*sizeof(int)); //allocation
            limits[ID+1][0] = 0; //The ID's x upper limit
            limits[ID+1][1] = n_y; //The ID's y upper limit
        }

        //Align all threads for the next part
        #pragma omp barrier

        //################### END OF PRE PROCESSING

        clock_t start = clock(); //START CLOCK FOR COMPUTATION TIME OF EACH THREAD

        //Now, we can simply let each thread compute the number of iterations for the elements of the rows assigned to it.
        for(int j=limits[ID][1];j<limits[ID+1][1];j++){
            for(int i=0;i<n_x;i++){
                valueMat[j][i] = MandelbrotSetTest(pixelPosX[i],pixelPosY[j],Imax);
            }
        }

        clock_t stop = clock(); //END CLOCK FOR COMPUTATION TIME OF EACH THREAD

        //The computation time of each thread is printed for testing reasons.
        printf("EXECUTION TIME FOR THREAD %d: %.0f ms\n",ID,((double)(stop-start)*1000.0/CLOCKS_PER_SEC));
    }

    //Saving all values into the file
    if(file!=NULL){
        //NOTE: The representation is made in the opposite sense along the row otherwise the final results will be y mirrored
        for(int j=n_y-1;j>=0;j--){
            for(int i=0;i<n_x-1;i++){
                fprintf(file,"%d,",valueMat[j][i]);
            }
        fprintf(file,"%d\n",valueMat[j][n_x-1]);
        }
    }
    else{ //If there was some problem with the file opening raise an error
        fprintf(stderr,"Output file opening results in NULL\n");
        exit(-9);
    }

    //Free the array and close the file
    free(pixelPosX);
    free(pixelPosY);
    free(valueMat);
    free(limits);
    fclose(file);

    //GENERAL EXECUTION TIME STOP
    clock_t BIGSTOP = clock();

    //The computation time of the whole process is printed for testing reasons.
    printf("EXECUTION TIME FOR ALL THE CODE: %.0f ms\n",((double)(BIGSTOP-BIGSTART)*1000.0/CLOCKS_PER_SEC));
    
    
	return(0);
}

//################# INPUT CHECK FUNCTION #############################
//This function will check if the inputs given during execution are meaning full
void argumentCheck(int argc,char* argv[]){

    //not enough arguments
    if(argc<=7){ 
        fprintf(stderr,"Usage: ./executable n_x n_y x_L y_L x_R y_R I_max\n");
        exit(-5);
    }

    //n_x and n_y must to be positive and greater or equal than 2.
    if((atoi(argv[1])<=1)||(atoi(argv[2])<=1)){ 
        fprintf(stderr,"Input Error: n_x or n_y less or equal than 1. They have to be greater or equal than 2!\n");
        exit(-2);
    }

    //The lower left corner of the image must be geometrically the lower left and so on the upper right!
    if((atof(argv[3])>=atof(argv[5]))||(atof(argv[4])>=atof(argv[6]))){
        fprintf(stderr,
                "Input Error: left corner have impossibile coordinates compared to the right corner. L: %d,%d. R:%d,%d",
                atof(argv[3]),atof(argv[4]),atof(argv[5]),atof(argv[6]));
        exit(-3);
    }

    //I_max must to be at least bigger than 1
    if(atoi(argv[7])<1){
        fprintf(stderr,"Input Error: I_max must to be bigger than 1.");
        exit(-4);
    }
    
    return;
}

//######################### MANDLEBROT SERIES ##################################
//All of these functions and structs are used for the Mandelbrot series computations

//It's easier and tidier to define a struct to represent complex numbers
typedef struct{
	float x; //real part
	float y; //imaginary part
}Complex;

//This function, given a complex number, check if it satisfies the Mandelbrot condition. 
//NOTE: the condition is checked in squared form to optimize performances.
bool MandelCond(Complex z){
    return((z.x*z.x+z.y*z.y)<4);
}

//This function given the starting point and the previous element of the series compute the next complex
//of the Mandelbrot series.
Complex MandelCalculus(Complex z,Complex c){
    Complex a;
    a.x = z.x*z.x-z.y*z.y+c.x; //the real part
    a.y = 2*z.x*z.y+c.y; //the imaginary part
    return(a);
}

//This central function, given the real and imaginary part of the starting complex number and the maximum number of iterations,
//computes how many iterations are necessary before the Mandelbrot series does not satisfies its condition.
unsigned int MandelbrotSetTest(float x,float y,unsigned int N){
    unsigned int result = 0;

    Complex z = {0.0,0.0};
    Complex c = {x,y};

    while((MandelCond(z))&&(result<N)){
        z = MandelCalculus(z,c);
        result++;
    }
    
    return result;
}
