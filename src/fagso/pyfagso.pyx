"""
This file is part of FAGSO, a program to find network partitions

  Copyright (C) 2014-2015 Carlo Nicolini <carlo.nicolini@iit.it>

  FAGSO is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  Alternatively, you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version.

  FAGSO is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
  GNU General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License and a copy of the GNU General Public License along with
  FAGSO. If not, see <http://www.gnu.org/licenses/>.
"""

from __future__ import division
import numpy as np
cimport numpy as np

# Cython imports
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp.vector cimport vector
import cython

ctypedef map[string, string] params_map

cdef extern from "Fagso.h":
    cdef cppclass Fagso:
        Fagso(double *A, int n, params_map params)
        void run()
        vector[int] getMembership()
        double getFinalSignificance()
        double getFinalSurprise()
        double getFinalAsymptoticSurprise()


@cython.boundscheck(False)
@cython.wraparound(False)
def fagso(np.ndarray[double, ndim=2, mode="c"] input not None, **kwargs):
    """
    FAGSO: Fast Agglomerative Surprise/Significance optimization
    
    Example:
      import networkx as nx
      from fagso import *
      G = nx.karate_club_graph()
      A  = nx.to_numpy_matrix(G)
      m,surp,sign = fagso(A, sort_method=8, repetitions=10)

    
    Usage:
        membership, surprise, significance = fagso(A, **kwargs)

    Args: 
        input: Adjacency matrix of the graph as a numpy 2D array, symmetric, binary.
               Can be cast from a Networkx graph G using "nx.to_numpy(G)"
    Kwargs:
        sort_method: sorting method for the edges, (default 0:Jaccard):
            0: Jaccard,
            1: AdamicAdar,
            2: AverageDegree,
            3: CommonNeighbors,
            4: HubDepressed ,
            5: HubPromoted,
            6: LeichtHolmeNewman,
            7: PreferentialAttachment,
            8: Random,
            8: ResourceAllocation,
            9: Salton,
            10: EdgeLength,
            11: Sorensen,
            12: JaccardNormalized,
            13: Cosine

        repetitions: number of repetitions of FAGSO (different results for randomized ranking of edges), (default 1).
        kfold: number of folds within randomization is carried, can help yielding better partitions, (default 1)
        verbosity: level of verbosity of output:  ERROR=0, WARNING=1, INFO=2, DEBUG=3, DEBUG1=4, DEBUG2=5, DEBUG3=6, DEBUG4=7 (default 0)
        seed: random seed for randomization (integer value)
        first_edge: index of the edge to be put as first edge to start agglomeration (default -1)
        save_solution: 1 to save ongoing solution, 0 otherwise (default 0)
        quality_function: wheter to maximize Surprise (0) or Significance (1), Asymptotic Surprise (2) or Asymptotic Modularity (3) (default 0)

    Out:
        membership: a list of vertices community membership
        surprise: the value of Surprise for the current partition
        significance: the value of Significance for the current partition
    """

    args = ['repetitions','kfold','verbosity', 'seed', 'first_edge',
            'quality_function', 'sort_method']

    args_diff = set(kwargs.keys()) - set(args)
    if args_diff:
        raise Exception("Invalid args:" + str(tuple(args_diff)) + "as input: valid arguments are " + str(args))

    cdef params_map par
    par[str("repetitions")] = str(kwargs.get("repetitions",1))
    par[str("kfold")] = str(kwargs.get("kfold",1))
    par[str("filename")] = "graph"
    par[str("sortmethod")] = str(kwargs.get("sort_method", 0))
    par[str("method")] = str(kwargs.get("method", 0))
    par[str("verbosity")] = str(kwargs.get("verbosity", 0))
    par[str("seed")] = str(kwargs.get("seed", -1))
    par[str("firstedge")] = str(kwargs.get("first_edge", -1))
    par[str("savesolution")] = str(kwargs.get("save_solution",0))
    par[str("qualityfunction")] = str(kwargs.get("quality_function",0))

    cdef Fagso* fagso = new Fagso( &input[0,0], input.shape[0], par)
    membership = fagso.getMembership()
    finalSurprise = fagso.getFinalSurprise()
    finalSignificance = fagso.getFinalSignificance()
    finalAsymptoticSurprise = fagso.getFinalAsymptoticSurprise()

    #del fagso
    return membership, finalSurprise, finalSignificance, finalAsymptoticSurprise

