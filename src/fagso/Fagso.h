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

#ifndef FAGSO_H
#define FAGSO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <set>
#include <map>
#include <Eigen/Core>
#include <stdexcept>
#include "Surprise.h"
#include "AsymptoticSignificance.h"
#include "AsymptoticSurprise.h"
#include "AsymptoticModularity.h"
#include "AsymptoticEntropy.h"
#include "SimilarityIndices.h"
#include "DisjointSet.h"
#include "Util.h"
#include "Timer.h"
#include "RandomShufflePercentage.h"

#include "FileLogger.h"


using namespace Eigen;
using std::cout;
using std::endl;
using std::cerr;
using std::set;
using std::string;
using std::pair;
using std::map;

/**
 * @brief randomTieBreak
 * @param l1
 * @param l2
 * @return
 */
inline bool sortLinksDescendingSimilarity(const pair<double, Link> &l1, const pair<double, Link> &l2)
{
    return l1.first>l2.first;
}


template <typename M, typename V>
void map2vec( const  M & m, V & v )
{
    for( typename M::const_iterator it = m.begin(); it != m.end(); ++it )
    {
        v.push_back( it->second );
    }
}

/**
 * @brief The Fagso class
 */
class Fagso
{
public:
    Fagso() {};
    ~Fagso() {};
    Fagso(double *pA, int nrows, std::map<std::string, std::string> param_map);
    Fagso(const GraphAdj &Adj, const Parameters &p);
    void init(const GraphAdj &A, const Parameters &params);
    void sumIntraclusterPairsAndEdges(const VertexCluster &clu, double *mc, double *pc, double *dc);
    void foldshuffle(LinksList &links, size_t nFolds);
    void sortLinks(LinksList &links);
    LinksList getEdges();

    double getSurprise(const DisjointSet &partition, int n, int m);
    double getAsymptoticSurprise(const DisjointSet &partition, int totalNodes, int totalEdges);
    double getSignificance(const DisjointSet &partition, int n, int totalEdges);
    double getModularity(const DisjointSet &partition, int totalEdges);
    double getModularityAsympt(const DisjointSet &partition, int n, int totalEdges);

    pair<double,DisjointSet> runFagsoSingleIter();
    void run();

    std::vector<int> getMembership();

    double getFinalSurprise();
    double getFinalSignificance();
    double getFinalAsymptoticSurprise();
    double getFinalModularity();
    double getFinalQuality();

protected:
    DisjointSet partition;
    Parameters params;
    GraphAdj A;
    long nVertices;
    long nEdges;
    LinksList edges;
    double finalSurprise;
    double finalSignificance;
    double finalAsymptoticSurprise;
    double finalModularity;
    double finalAsymptoticModularity;
};


#endif // FAGSO_H
