### gather/EPYC/Size

To study the latency for a varying size of the message fixed the number of processes to 256,
the "osu_gather" benchmark was run on the two entirely reserved EPYC nodes (256 cores), using a 
mapping by node with a number of processes equal to the number of cores. The latency
was studied for the base case and, then, specifying one of 3 algorithms for "gather":

- linear (called "linear")
- binomial (called "binomial")
- linear with synchronisatiion (called "sync")

For each algorithm, the benchmark was runned 20 times and then, using a Python pipeline (output_elaborator.py),
the bash output scripts were converted in CSV files. 