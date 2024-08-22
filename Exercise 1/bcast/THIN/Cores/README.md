### bcast/THIN/Cores

To study the latency for a varying number of processes the "osu_bcast" benchmark was run
on the two entirely reserved THIN nodes (48 cores) using a mapping by node. The latency
was studied for the base case and then specifying one of 3 algorithms for "bcast":

- pipeline (called "pipeline")
- binomial (called "binomial")
- scatter_allgather_ring (called "ring")

For each number of processes the benchmark was runned 20 times and using a Python pipeline (output_elaborator.py)
the bash output scripts were converted in CSV files that you can find the subdirectories. 

To collect data for each number of processes from 2 to 48 I have repeated the 20 runs.