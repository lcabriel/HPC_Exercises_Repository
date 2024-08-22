### gather/THIN/Cores

To study the latency for a varying number of processes, the "osu_gather" benchmark was run
on the two entirely reserved THIN nodes (48 cores) using a mapping by node. The latency
was studied for the base case and, then, specifying one of 3 algorithms for "gather":

- linear (called "linear")
- binomial (called "binomial")
- linear with synchronisation (called "sync")

For each number of processes the benchmark was runned 20 times and then, using a Python pipeline (output_elaborator.py),
the bash output scripts were converted in CSV files that you can find the subdirectories. 