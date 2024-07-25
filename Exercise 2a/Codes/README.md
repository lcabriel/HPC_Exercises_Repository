## Exercise 2a: Codes

In this directory you can find all the C codes used to solve the algorithm and to perform the tests. The final version of the algorithm is "broadcast_refactor2.c" howevere I left all the other versions for compaharison. The broadcast codes contains inside the alternative broadcast algorithm but also a little test which is used to perform the analysis of the computation times.
Now I will briefly describe all the codes but for more information please look to the report and to the comments inside each one of them.

- "broadcast.c": A first initial version of the code. All the elements are present still the logic is not optimize and too many checks are performed. No optimization are performed. The tree of processes structure is already implemented with the possibility to broadcast from different nodes and to broadcast also arrays of values.
- "broadcast_refactor1.c": some general improvements on memory accesses are implemented. The main feature is the improvement of the logic both under the performance POV both completly some missing logic.
- "broadcast_refactor2.c": in this version I focused on improving the number of accesses in the "initializeProcessTree" function. Using an alternative logic the new algorithm allow to access only one time to each node to set them thanks to the tree structure. NOTE FOR COMPILE: this code us the **math** library!

The other C code in the directory ("broadcast_native.c") is strictly used for the compaharisons. The final results is the same but this time is done using the official broadcast method of OpenMPI.
