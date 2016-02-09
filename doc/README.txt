FAGSO
Fast Agglomerative Surprise Optimization

FAGSO is a tool for community detection based on Surprise maximization. It explicitly maximizes Surprise.

Version 0.1.0
February, 9, 2016

If you use this software, please cite:
Nicolini C. and Bifone A. "Modular structure of brain functional networks: breaking the resolution limit by Surprise", Scientific Report

Table of contents
=================
- System requirements
- How to use
- Using pure Python FAGSO
- Using compiled Python interface
- Examples
- Troubleshooting

System requirements
===================

Pure Python FAGSO requires networkx to be installed. On Ubuntu is very simple:

$> sudo apt-get install python-networkx

Compiled Python FAGSO does not need networkx but a g++ compiler, newer than g++-4.6

Currenly compiled FAGSO is available only for Linux 64, OSX support coming soon.


How to use pure Python FAGSO
============================

Pure Python:

	1. Start a python console, import networkx and fagso 

	>> import networkx as nx
	>> from fagso import fagso
	
	Then generate a benchmark graph:

	>> G = nx.karate_club_graph()
	
	and run fagso on G. *fagso* returns as first argument a dictionary where keys are the vertices and values are the community indices of vertices. Second argument is the numeric value of Surprise.

	>> [membership, surprise_value] = fagso(G)


How to use compiled Python FAGSO
================================

Compiled FAGSO works directly with the adjacency matrix of the graph, provided as a numpy matrix. It **does not** need to install networkx.

IMPORTANT: Please ensure the matrix is symmetric and binary. To do so in numpy, if you load a weighted adjacency matrix, you can convert it in binary form with the following command:

	>> A=np.loadtxt('some_real_matrix.txt')
	>> B=(A>0).astype(int).astype(float)

*** It's important that the input matrix is a float matrix of 1s and 0s ***

	1. Start a python console

	>> import numpy as np
	>> from pyfagso import fagso
	>> A=np.loadtxt('karate.adj')
	>>  [membership,surprise]=fagso(A)

	Fagso returns as first argument a dictionary where keys are the vertices and values are the community indices of vertices. Second argument is the numeric value of Surprise.


Troubleshooting
===============

Q. Importing pyfagso outputs the error:

/usr/lib/x86_64-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.20' not found (required by ./pyfagso.so)

A. This means that the current version of pyfagso is compiled against a newer compiler than the one installed on your system. Please contact me with the version of your compiler, otherwise install any g++ compiler newer than g++-4.6. 


Q. I get `ValueError: Buffer dtype mismatch, expected 'double' but got 'long'`
A. You must convert your matrix as a numpy 2D square array with float elements with values 1 and 0.

ValueError: Buffer dtype mismatch, expected 'double' but got 'long'
You can do it with:

If your matrix `mymatrix.txt` is like this:

   17.30322    4.00067    0.00000    0.00000
    4.00067    0.00000    0.00000    0.14667
    0.00000    0.00000   12.67823    6.87591
    0.00000    0.14667    6.87591    0.00000

then the input matrix to FAGSO should appear like:

[[ 1.0, 1.0, 0.0, 0.0],
[  1.0, 0.0, 0.0, 1.0],
[  0.0, 0.0, 1.0, 0.0],
[  0.0, 1.0, 0.0, 0.0]]

Example:
>> A=np.loadtxt('mymatrix.txt')
>> B=(A>0).astype(int).astype(float)
