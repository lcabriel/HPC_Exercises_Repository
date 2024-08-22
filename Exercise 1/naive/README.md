## naive

In this directory, you can find the time data that will be used to obtain the
execution time for the naive approach. The naive approach uses no structure but
will simple transmit from a core to all the other ones using a point to point
comunication without algorithms or structures.

The naive time data will be used, as example, for a fixed number of processes
and varying message size multiplying the execution time for the different sizes
by the number of processes minus 1. 

This data were collected using the OSU benchmark "osu_latency" reserving, as
always, the two nodes with all the cores and mapping the two processes of the
comunication by node. To stabilize the results, I have repeated for the two
types of node 20 times the benchmark.

With this data I have elaborated it with a Python pipeline (output_elaborator.py)
.
We have stored separately the data concerning the EPYC nodes and THIN nodes
in the two CSV files. 