BFS was used to find all shortest path in the graph. 

Data parallelism was used to make the application to compute in parallel.

Each thread has its own set of vertices for what it each thread computes the 

shortest path and and finds "betweeness". The main thread generates the random 

graph beforehand. 

real    0m0.027s
user    0m0.003s
sys     0m0.003s

real    0m0.016s
user    0m0.002s
sys     0m0.002s

real    0m0.012s
user    0m0.002s
sys     0m0.004s


real    0m0.013s
user    0m0.007s
sys     0m0.000s




