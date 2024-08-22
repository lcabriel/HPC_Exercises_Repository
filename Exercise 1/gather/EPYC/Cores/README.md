### gather/EPYC/Cores

To study the latency for a varying number of processes, the "osu_gather" benchmark was run
on the two entirely reserved EPYC nodes (256 cores) using a mapping by node. The latency
was studied for the base case and, then, specifying one of 3 algorithms for "gather":

- linear (called "linear")
- binomial (called "binomial")
- linear with synchronisation (called "sync")

For each number of processes, the benchmark was runned 20 times and then, using a Python pipeline (output_elaborator.py),
the bash output scripts were converted in CSV files that you can find the subdirectories. 

Collect data for each number of processes from 2 to 256 would be too much time consuming and,
also, not so significative, therefore, starting from 2 processes, I have studied until 254 moving
with a step of 6 processes. For this reason, the number notation of the files is completely
uncorrelated to the number of processes used. 