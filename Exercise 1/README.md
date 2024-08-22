# Exercise 1

For the Exercise 1, I opted to analyse the two collective operations "bcast" and "gather". For the two nodes,
I have collected the latency data on EPYC and THIN nodes. In particular, the nodes used for this exercise where
epyc005 and epyc006 for the EPYC. For the THIN nodes, I have collected data on thin006 and thin007.

For each operation and for each couple of nodes, I have reserved entirely the two nodes and then I have studied
the latency in two situations:

- **Fixed number of processes but varying size of the message**: in this first analysis I have fixed the number of
process equal to the number of cores and, then, I have collected the data for the different sizes of message passed.
- **Fixed size of the message and varying number of processes**: still reserving all the cores in the 2 studied nodes, I have
performed the analysis collecting the latency for the maximum size of the message but for a varying number of processes,
from 2 to the number of cores (or almost in EPYC).

In all the computations I have always mapped by node.
For each collective operation and for the two analysis, I have gathered latency data not only in the "base" case but specifying
also some algorithms. In particular I have choose 3 for each operation:

- **bcast**: pipeline, binomial and scatter_allgather_ring
- **gather**: basic linear, binomial, linear with synchronization

Therefore, for each operation, in each node and for each algorithm, I have collected the times produced by 20 runs of the corresponding
OSU benchmarks (in particular "osu_bcast" and "osu_gather"). The data (obtained using bash scripts) were elaborated using some Python
 pipelines which codes are left inside the subdirectories. To transform the data in plots, I have also used Python and you can find
 the commented results in the Python Notebook "times.ipynb" or inside the report. Inside this report, the times are also confronted with
 the naive approach based on the point to point communication latency data obtained in a similar way using the OSU benchmark "osu_latency".

To look deeper into the data, they are all available in the subdirectories:

- **bcast**: here are stored all the time data about the bcast operation
- **gather**: here are stored all the time data about the gather operation
- **naive**: here are stored all the time data about point to point communication