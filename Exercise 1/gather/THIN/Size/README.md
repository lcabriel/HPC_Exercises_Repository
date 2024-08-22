### gather/THIN/Size

To study the latency for a varying size of the message fixed the number of processes to 48,
the "osu_gather" benchmark was run on the two entirely reserved THIN nodes (48 cores) using a 
mapping by node with a number of processes equal to the number of cores. The latency
was studied for the base case and then specifying one of 3 algorithms for "gather":

- linear (called "linear")
- binomial (called "binomial")
- linear with synchronisation (called "sync")

For each algorithm, the benchmark was runned 20 times and using a Python pipeline (output_elaborator.py)
the bash output scripts were converted in CSV files. 