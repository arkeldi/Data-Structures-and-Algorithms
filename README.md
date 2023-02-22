# <p align="center">Prim's and Disjsktra's Algorithm Program<p>

**Introduction**

This is a program of a popular known algorithm: Prim's and Disjsktra's. The website here provides a great explanation: https://medium.com/@shagunaawasthi/prims-implementation-using-dijkstra-230355b8352a

My program is essebtially a map of maps, allowing for direct indexing with low memory usage. Then the algorithms follow the same logic, and uses a trick regarding the priority queue which works well. Performance is the minimum bound of these algorithms. My implementation is O(NLogn). 

The graph.h file declares the members and functions of the class to be used in the test file. It provides the functions for Prim's and Dijsktra's Algorithms as well.

The graph_tests.cpp file contains the main driver program for the Project. This file provides test cases for the Graph class and all it's functions. ALl test values are hard coded since there is no user input. It prints to screen the of the number of vertices, edges, and both Prim & Dijsktra's shortest paths from the given vertice and the distance.

I recieved a perfect score for this end of semester project graded on code correctness 60/60 points, test cases 30/30 points, and coding standards 10/10 points. 
