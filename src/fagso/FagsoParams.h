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


#ifndef FAGSOPARAMS_H
#define FAGSOPARAMS_H

#include <vector>
#include <Eigen/Core>

#include "Graph.h"
/**
 * @brief The SortMethod enum
 */
enum SortMethod
{
    SortJaccard,
    SortAdamicAdar,
    SortAverageDegree,
    SortCommonNeighbors,
    SortHubDepressed ,
    SortHubPromoted,
    SortLeichtHolmeNewman,
    SortPreferentialAttachment,
    SortRandom,
    SortResourceAllocation,
    SortSalton,
    SortSorensen,
    SortJaccardNormalized,
    SortCosine,
    num_values = 15
};

enum CostFunction
{
    MAX_SURPRISE_DISCRETE = 0,
    MAX_SIGNIFICANCE_ASYMPT = 1,
    MAX_SURPRISE_ASYMPT = 2,
    MAX_MODULARITY = 3,
    MAX_MODULARITY_ASYMPT = 4
};

static const char * const costFunctionNames[] =
{
    "Surprise",
    "Significance",
    "AsymptoticalSurprise",
    "AsymptoticalModularity"
};

static const char * const sortMethodNames[] =
{
    "Jaccard",
    "AdamicAdar",
    "AverageDegree",
    "CommonNeighbors",
    "HubDepressed",
    "HubPromoted",
    "LeichtHolmeNewman",
    "PreferentialAttachment",
    "Random",
    "ResourceAllocation",
    "Salton",
    "Sorensen",
    "SortJaccardNormalized",
    "SortCosine"
};


/**
 * @brief The Parameters struct
 */
struct Parameters
{
    Parameters() :
        repetitions(1),
        kfold(1),
        firstEdge(0),
        costFunc(MAX_SURPRISE_DISCRETE),
        sortmethod(SortJaccard),
        printsolution(false),
        initialRandomize(false),
        saveSolution(true),
        seed(-1),
        verbosity(0),
        shufflePercentage(0.0)
    {
    }
    std::string inputGraphFileName;
    std::string partitionFileName;
    std::string outputFolder;
    std::string coordinateFileName;
    size_t repetitions;
    size_t kfold;
    size_t firstEdge;
    CostFunction costFunc;
    SortMethod sortmethod;
    bool printsolution;
    bool initialRandomize;
    bool saveSolution;
    int seed;
    int verbosity;
    double shufflePercentage;
};

#endif // FAGSOPARAMS_H
