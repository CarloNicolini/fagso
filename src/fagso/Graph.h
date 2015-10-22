/* This file is part of FAGSO, a program to find network partitions
*
*  Copyright (C) 2014-2015 Carlo Nicolini <carlo.nicolini@iit.it>
*
*  FAGSO is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 3 of the License, or (at your option) any later version.
*
*  Alternatively, you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of
*  the License, or (at your option) any later version.
*
*  FAGSO is distributed in the hope that it will be useful, but WITHOUT ANY
*  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*  FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License and a copy of the GNU General Public License along with
*  FAGSO. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <Eigen/Core>

using Eigen::MatrixXf;

typedef std::pair<int, int> Link;
typedef std::vector<Link> LinksList;
typedef Eigen::MatrixXd GraphAdj;

typedef std::map<int,int> VertexMembership;
typedef std::set<int> VertexCluster;

typedef std::vector<int> VVertexMembership;
typedef std::vector<int>::iterator VVertexMembershipIterator;
typedef std::vector<int>::const_iterator VVertexMembershipConstIterator;

typedef std::set<int>::iterator VertexClusterIterator;
typedef std::set<int>::const_iterator VertexClusterConstIterator;
typedef std::map<int, std::set<int> > VertexClustering;

typedef VertexMembership::iterator VertexMembershipIterator;
typedef VertexMembership::const_iterator VertexMembershipConstIterator;
typedef VertexClustering::iterator VertexClusteringIterator;

typedef VertexMembership::const_iterator VertexMembershipConstIterator;
typedef VertexClustering::const_iterator VertexClusteringConstIterator;

GraphAdj erdos_renyi_gnm_random_graph(int n, int m);

/**
 * @brief read_graph
 * @param filename
 * @return
 */
GraphAdj readAdjacencyMatrix(const std::string &filename);

/**
 * @brief readDimacs
 * @param filename
 * @return
 */
GraphAdj readDimacs(const std::string &filename);

/**
 * @brief loadCoordinates
 * @param filename
 * @return
 */
MatrixXf loadCoordinates(const std::string &filename);

/**
 * @brief membership2clustering
 * @param m
 * @return
 */
VertexClustering membership2clustering(const VertexMembership &m);

#endif
