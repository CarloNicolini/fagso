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


#ifndef SIMILARITYINDICES_H
#define SIMILARITYINDICES_H
#include <set>
#include <Eigen/Core>
#include "DisjointSet.h"
#include "FagsoParams.h"


/**
 * @brief getNeighbors
 * @param A
 * @param node
 * @return
 */
std::set<int> getNeighbors(const GraphAdj &A, int node);

/**
 * @brief getDegree
 * @param A
 * @param node
 * @return
 */
size_t getDegree(const GraphAdj &A, int node);

/**
 * @brief getIntraDegree
 * @param A
 * @param ds
 * @param node
 * @return
 */
size_t getIntraDegree(const GraphAdj &A, DisjointSet &ds, int node);

/**
 * @brief jaccardIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double jaccardIndex(const GraphAdj &A, int n1, int n2);

/**
 * @brief jaccardNormalized
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double jaccardNormalized(const GraphAdj &A, int n1, int n2);


/**
 * @brief cosineIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double cosineIndex(const GraphAdj &A, int n1, int n2);


/**
 * @brief commonNeighbors
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double commonNeighbors(const GraphAdj &A, int n1, int n2);

/**
 * @brief saltonIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double saltonIndex(const GraphAdj &A, int n1, int n2);

/**
 * @brief hubPromotedIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double hubPromotedIndex(const GraphAdj &A, int n1, int n2);

/**
 * @brief hubDepressedIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double hubDepressedIndex(const GraphAdj &A, int n1, int n2);

/**
 * @brief resourceAllocationIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double resourceAllocationIndex(const GraphAdj &A, int n1, int n2);

/**
 * @brief leightHolmeNewmanIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double leightHolmeNewmanIndex(const GraphAdj &A, int n1, int n2);

/**
 * @brief adamicAdarIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double adamicAdarIndex(const GraphAdj &A, int n1, int n2);

/**
 * @brief preferentialAttachmentIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double preferentialAttachmentIndex(const GraphAdj &A, int n1, int n2);


/**
 * @brief sorensenIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double sorensenIndex(const GraphAdj &A, int n1, int n2);

/**
 * @brief averageDegreeIndex
 * @param A
 * @param n1
 * @param n2
 * @return
 */
double averageDegreeIndex(const GraphAdj &A, int n1, int n2);
#endif // SIMILARITYINDICES_H
