/* This file is part of FAGSO, a program to find network partitions
*
*  Copyright (C) 2014-2015 Carlo Nicolini <carlo.nicolini@iit.it>
*
*  Surprise++ is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 3 of the License, or (at your option) any later version.
*
*  Alternatively, you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of
*  the License, or (at your option) any later version.
*
*  Surprise++ is distributed in the hope that it will be useful, but WITHOUT ANY
*  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*  FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License and a copy of the GNU General Public License along with
*  Surprise++. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _DISJOINT_SET_H_
#define _DISJOINT_SET_H_
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "Graph.h"


class DisjointSet
{
public:
    DisjointSet();
    DisjointSet(int N, bool randomize=false);
    DisjointSet(const VertexMembership &);
    DisjointSet(const DisjointSet&);

    // Membership of the vertices
    VertexMembership membership;
    VertexMembership oldMembership;
    // Facility to index node by index (not map)
    VVertexMembership vmembership;
    // Clustering, every node has its own cluster which it belongs to
    VertexClustering clustering;
    VertexClustering oldClustering;

    // Returns the community index to which p belongs
    int find(int p);
    void add(int p, int q);
    // Returns true if node p and q are in the same community
    bool connected(int p, int q);

    void undo(); // undo last merge

    void print(std::ostream &os);
    void save(const std::string &filename);
    void sortBySize();

    // Keep track of intracluster edges and pairs
    std::map<int, int> intraClusterEdges;
    std::map<int, int> intraClusterPairs;
};

#endif
