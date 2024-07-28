## Exercise 2a: Codes

In this directory you can find all the C codes used to solve the problem request. The final version of the algorithm is "mandelbrot_parallel_refactor.c" however I left all the other versions for comparison and also because are used to test the different performances of the different approaches.
Now I will briefly describe all the codes but for more information please look to the report and to the comments inside each one of them.

- "mandelbrot_simple.c": A first initial version of the code. This serial version of the code solves the problem request in a very unoptimized approach. Being serial, the code simply start from the lower left corner of the domain and, section by section (or pixel by pixel), apply the Mandelbrot series recording the number of iterations until a cap is reached or the Mandelbrot condition is no longer satisfied. 
- "mandelbrot_parallel.c": This unoptimized code tries to implement a multi-thread approach to solve the problem. The algorithm is made to assign in some way defined regions to certain thread. The regions must be connected. The particular version implemented in the code simply splits the rows in a number of equal size regions equal to the number of threads. Doing so the first thread will have the first $N/nthreads$ rows and so on.
- "mandelbrot_parallel_refactor.c": In thi final version of the code, a more intelligent algorithm is implemented. Due to the different weight of execution times of the rows the previous code produces very asymmetric computation times for the threads. The algorithm in this final versions allows a fairer division of the work load between the threads. Some improvements in the memory usage have also been implemented

All the C codes need the standard **math.h** library to work correctly in addition to OpenMP  
