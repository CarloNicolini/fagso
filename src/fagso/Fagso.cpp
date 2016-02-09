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

#include "Fagso.h"

/**
 * @brief Fagso::Fagso
 * @param Adj
 * @param p
 */
Fagso::Fagso(const GraphAdj &Adj, const Parameters &p)
{
    this->init(Adj,p);
}

/**
 * @brief Fagso::Fagso
 * @param pA
 * @param nrows
 * @param param_map
 */
Fagso::Fagso(double *pA, int nrows, std::map<std::string,std::string> param_map)
{
    this->init(Eigen::Map<GraphAdj>(pA,nrows,nrows), this->params);
    {
        params.sortmethod = SortJaccard;
        params.kfold = 1;
        params.repetitions=1;
        params.firstEdge=-1;
        params.printsolution=false;
        params.saveSolution=false;
        params.initialRandomize=false;
        params.costFunc = MAX_SURPRISE_DISCRETE;
        params.seed = -1;
        params.verbosity = 0;
    }

    this->params.seed = str2int(param_map["seed"]);
    if (this->params.seed == -1)
        randomizeStart();
    this->params.inputGraphFileName = "graph";
    this->params.firstEdge = str2int(param_map["firstedge"]);
    this->params.kfold = str2int(param_map["kfold"]);
    this->params.partitionFileName = "graph";
    this->params.sortmethod = static_cast<SortMethod>(str2int(param_map["sortmethod"]));
    this->params.repetitions = str2int(param_map["repetitions"]);
    this->params.verbosity = str2int(param_map["verbosity"]);
    this->params.saveSolution = str2int(param_map["savesolution"]);
    this->params.costFunc = static_cast<CostFunction>(str2int(param_map["qualityfunction"]));

    FILELog::ReportingLevel() = static_cast<TLogLevel>(params.verbosity);
    for (std::map<string,string>::iterator iter = param_map.begin(); iter!=param_map.end(); ++iter)
    {
        FILE_LOG(logDEBUG2) << "Parameter \"" << iter->first << "\"=\"" << iter->second << "\"";
    }

    this->run();
}

/**
 * @brief Fagso::init
 * @param Adj
 * @param params
 */
void Fagso::init(const GraphAdj &Adj, const Parameters &params)
{
    this->A = Adj;
    this->params = params;
    this->nVertices = Adj.cols(); // number of nodes
    this->edges = getEdges();
    this->nEdges = this->edges.size();
    // Initialize the disjoint set that contains the solution
    this->partition = DisjointSet(this->A.cols(),params.initialRandomize);
    FILE_LOG(logINFO) << "Graph (V,E)=" << nVertices << " " << nEdges;
}

/**
 * @brief Fagso::sumIntraclusterPairsAndEdges
 * @param clu
 * @param mi
 * @param pi
 */
void Fagso::sumIntraclusterPairsAndEdges(const VertexCluster &clu, double *mc, double *pc, double *dc)
{
    int ni = clu.size(); // intracluster vertices
    int submi = 0; // start counting intracluster edges
    *pc += static_cast<double>(ni*(ni-1)/2); // intracluster pairs
    for ( VertexClusterConstIterator iInd1 = clu.begin(); iInd1!=clu.end(); ++iInd1)
    {
        for ( VertexClusterConstIterator iInd2 = iInd1; iInd2!=clu.end(); ++iInd2)
            submi += (A.coeffRef(*iInd1,*iInd2)!=0);
        *dc += A.row(*iInd1).sum(); // intracluster degree
    }
    *mc += submi;
}

#include <algorithm>    // std::unique, std::distance
/**
 * @brief Fagso::getSurprise
 * @param ds
 * @param nNodes
 * @param nEdges
 * @return The surprise of the partition contained in Disjoint set ds with nNodes and nEdges
 */
double Fagso::getSurprise(const DisjointSet &ds, int n, int m)
{
    double p = n*(n-1)/2;
    double mi=0.0;
    double pi=0.0;

    for (LinksList::const_iterator iter = this->edges.begin(); iter!=this->edges.end(); ++iter)
    {
        int node1 = iter->first;
        int node2 = iter->second;
        int commfrom = ds.membership.at(node1);
        int commto = ds.membership.at(node2);
        if (commfrom==commto)
            mi += 1;
    }

    // Iterate over each cluster in the clustering
    for (VertexClusteringConstIterator iter = ds.clustering.begin(); iter!=ds.clustering.end(); ++iter)
    {
        int ni = (iter->second.size());
        pi += ni*(ni-1)/2; // Sum the intracluster pairs
    }
    double S = computeSurprise(p,pi,m,mi);
    FILE_LOG(logDEBUG4) << "Surprise =" << S;
    return S;
}

/**
 * @brief Fagso::getAsymptoticSurprise
 * @param ds
 * @param nNodes
 * @param nEdges
 * @return The surprise of the partition contained in Disjoint set ds with nNodes and nEdges
 */
double Fagso::getAsymptoticSurprise(const DisjointSet &ds, int n, int m)
{
    double p = n*(n-1)/2;
    double mi=0.0;
    double pi=0.0;

    for (LinksList::iterator iter = this->edges.begin(); iter!=this->edges.end(); ++iter)
    {
        int commfrom = ds.membership.at(iter->first);
        int commto = ds.membership.at(iter->second);
        if (commfrom==commto)
            mi += 1; // sum intracluster edges
    }

    // Iterate over each cluster in the clustering
    for (VertexClusteringConstIterator iter = ds.clustering.begin(); iter!=ds.clustering.end(); ++iter)
    {
        int ni = (iter->second.size());
        pi += ni*(ni-1)/2; // Sum the intracluster pairs
    }

    double S = computeAsymptoticSurprise(p,pi,m,mi);
    FILE_LOG(logDEBUG4) << "AsymptSurprise =" << S << " " << computeSurprise(p,pi,m,mi) << " " << S/computeSurprise(p,pi,m,mi);
    return S;
}

/**
 * @brief Fagso::getSignificance
 * @param ds
 * @param totalNodes
 * @param totalEdges
 * @return
 */
double Fagso::getSignificance(const DisjointSet &ds, int n, int m)
{
    double p = n*(n-1)/2.0;
    double rho = static_cast<double>(m)/p;
    double Significance = 0.0;
    // Iterate over each cluster in the clustering
    for (VertexClusteringConstIterator iter = ds.clustering.begin(); iter!= ds.clustering.end(); ++iter)
    {
        double mc=0.0; // i-th intracluster edges
        double pc=0; // i-th intracluster pairs
        double dc=0.0;
        sumIntraclusterPairsAndEdges(iter->second,&mc,&pc,&dc);
        double ni = iter->second.size(); // i-th intracluster nodes
        double rhoi = mc/pc;
        if (ni > 1)
        {
            Significance += asymptoticSignificance(ni,rhoi,rho);
        }
    }
    return Significance;
}

/**
 * @brief Fagso::getModularity
 * @param ds
 * @param n
 * @param m
 * @return
 */
double Fagso::getModularity(const DisjointSet &ds, int m)
{
    double modularity = 0.0;
    // Q_kl = sum_c KL(m_c/m || (d_c/2m)^2)
    // For every community
    double mzeta=0.0;
    double kc = 0.0;
    for (VertexClusteringConstIterator iter = ds.clustering.begin(); iter!= ds.clustering.end(); ++iter)
    {
        double mc=0.0; // i-th intracluster edges
        double pc=0.0;
        double dc = 0.0; //sum of degrees intracommunity
        // For every vertex in the community
        sumIntraclusterPairsAndEdges(iter->second,&mc,&pc,&dc);
        modularity += mc/double(m) - pow(dc/(2*m),2); // correct definition of modularity
        mzeta +=mc;
        kc+=dc;
    }
    FILE_LOG(logDEBUG4) << "Mod =" << modularity;
    FILE_LOG(logDEBUG4) << "KLMod =" << asymptoticModularity(mzeta,m,kc);
    return modularity;
}

/**
 * @brief Fagso::getModularityAsympt
 * @param ds
 * @param m
 * @return
 */
double Fagso::getModularityAsympt(const DisjointSet &ds, int n, int m)
{
    double modularityAsympt = 0.0;
    // Q_kl = sum_c KL(m_c/m || (d_c/2m)^2)
    // For every community
    double mzeta=0.0;
    double kc = 0.0;
    for (VertexClusteringConstIterator iter = ds.clustering.begin(); iter!= ds.clustering.end(); ++iter)
    {
        double mc=0.0; // i-th intracluster edges
        double pc=0.0;
        double dc = 0.0; //sum of degrees intracommunity
        // For every vertex in the community
        sumIntraclusterPairsAndEdges(iter->second,&mc,&pc,&dc);
        modularityAsympt += asymptoticModularity(mc,m,dc);
        mzeta +=mc;
        kc+=dc;
    }
    //double Qasympt = asymptoticModularity(mzeta,m,kc);
    return modularityAsympt;
}

/**
 * @brief Fagso::foldshuffle
 * @param links
 * @param nFolds
 * Shuffle random within each of the nFolds
 */
void Fagso::foldshuffle(LinksList &links, size_t nFolds)
{
    size_t foldsize = std::div(nEdges,nFolds).quot;
    for (size_t i=0; i<nFolds; ++i)
    {
        size_t foldStart = (i*foldsize);
        size_t foldEnd = ((i+1)*foldsize);
        std::random_shuffle(links.begin()+foldStart, links.begin()+foldEnd);
    }
}

/**
 * @brief Fagso::sortLinks
 * @param links
 * Sort the links in the order specified by the parameters
 */
void Fagso::sortLinks(LinksList &links)
{
    FILE_LOG(logDEBUG1) << "Sorting links, method " << params.sortmethod << ":\"" <<sortMethodNames[params.sortmethod] << "\"";
    if (links.empty())
        return;

    if (this->params.sortmethod == SortRandom)
    {
        std::random_shuffle(links.begin(),links.end());
        return;
    }

    std::vector < pair<double, Link > > linksIndex(links.size());

    for (size_t i=0; i<links.size(); ++i)
    {
        pair<double, Link> curLink(0.0,links.at(i));
        int n1 = curLink.second.first;
        int n2 = curLink.second.second;
        switch (this->params.sortmethod)
        {
        case SortAverageDegree:
        {
            curLink.first = averageDegreeIndex(A, n1, n2);
            break;
        }
        case SortSorensen:
        {
            curLink.first = sorensenIndex(A, n1, n2);
            break;
        }
        case SortJaccard:
        {
            curLink.first = jaccardIndex(A, n1, n2);
            break;
        }
        case SortCommonNeighbors:
        {
            curLink.first = commonNeighbors(A, n1, n2);
            break;
        }
        case SortAdamicAdar:
        {
            curLink.first = adamicAdarIndex(A, n1, n2);
            break;
        }
        case SortHubDepressed:
        {
            curLink.first = hubDepressedIndex(A, n1, n2);
            break;
        }
        case SortHubPromoted:
        {
            curLink.first = hubPromotedIndex(A, n1, n2);
            break;
        }
        case SortResourceAllocation:
        {
            curLink.first = resourceAllocationIndex(A, n1, n2);
            break;
        }
        case SortPreferentialAttachment:
        {
            curLink.first = preferentialAttachmentIndex(A, n1, n2);
            break;
        }
        case SortSalton:
        {
            curLink.first = saltonIndex(A, n1, n2);
            break;
        }
        case SortLeichtHolmeNewman:
        {
            curLink.first  = leightHolmeNewmanIndex(A, n1, n2);
            break;
        }
        case SortJaccardNormalized:
        {
            curLink.first = jaccardNormalized(A,n1,n2);
            break;
        }
        case SortCosine:
        {
            curLink.first = cosineIndex(A,n1,n2);
            break;
        }
        case SortRandom:
        case num_values:
        {
            break;
        }

        }
        linksIndex.at(i) = curLink;
    }

    // Sort the links by descending similarity
    std::sort(linksIndex.begin(),linksIndex.end(),sortLinksDescendingSimilarity);
    // Then randomize the edges with the same similarity value
    for (size_t i=0; i<linksIndex.size();++i)
    {
        std::vector < pair<double, Link > >::iterator beg = linksIndex.begin()+i;
        std::vector < pair<double, Link > >::iterator end = linksIndex.begin()+i;
        while ( (beg->first == end->first) && end<linksIndex.end())
        {
            ++end;
        }
        std::random_shuffle(beg,end);
    }

    // Change the first edge to redirect the search
    pair<double, Link> x = linksIndex.at(params.firstEdge%linksIndex.size());
    std::swap(x,linksIndex.front());

    // Repopulate the links
    for (size_t i=0; i<linksIndex.size(); ++i)
    {
        //cout << "(" << linksIndex[i].second.first << "- " << linksIndex[i].second.second << ") " << linksIndex[i].first << endl;
        links[i].first = linksIndex[i].second.first;
        links[i].second = linksIndex[i].second.second;
    }

    // Reshuffle in folds
    if (params.kfold!=1)
    {
        FILE_LOG(logDEBUG1) << "Shuffling links within "<< params.kfold << " folds";
        foldshuffle(links,params.kfold);
    }

    FILE_LOG(logDEBUG1) << "Sorting links finished.";
}

/**
 * @brief Fagso::getMembership
 * @return
 */
std::vector<int> Fagso::getMembership()
{
    std::vector<int> memb(partition.membership.size());
    for (std::map<int,int>::iterator iter = partition.membership.begin(); iter!=partition.membership.end(); ++iter)
    {
        memb.at(iter->first) = iter->second;
    }
    return memb;
}

/**
 * @brief Fagso::getEdges
 * @return
 */
LinksList Fagso::getEdges()
{
    LinksList edges;
    for (int i=0; i<this->nVertices; ++i)
    {
        for (int j=i+1; j<nVertices; j++)
        {
            if (A.coeffRef(i,j)!=0)
            {
                edges.push_back(pair<int,int>(i,j));
            }
        }
    }
    return edges;
}

/**
 * @brief Fagso::runFagsoSingleIter
 * @return
 */
pair<double,DisjointSet> Fagso::runFagsoSingleIter()
{
    int nVertices = A.cols(); // number of nodes
    DisjointSet curPartition(nVertices, this->params.initialRandomize);
    int iteration=0;
    LinksList edges = getEdges();
    sortLinks(edges);
    double qual=std::numeric_limits<double>::min();
    while (iteration < nEdges)
    {
        Link e = edges[iteration];
        if (curPartition.membership[e.first] != curPartition.membership[e.second]) // keep only intercluster edges
        {
            curPartition.add(e.first,e.second);
            double curQual = 0.0;
            switch (params.costFunc)
            {
            case MAX_SURPRISE_DISCRETE:
            {
                curQual = getSurprise(curPartition,nVertices,nEdges);
                break;
            }
            case MAX_SIGNIFICANCE_ASYMPT:
            {
                curQual = getSignificance(curPartition,nVertices,nEdges);
                break;
            }
            case MAX_SURPRISE_ASYMPT:
            {
                curQual = getAsymptoticSurprise(curPartition,nVertices,nEdges);
                break;
            }
            case MAX_MODULARITY:
            {
                curQual = getModularity(curPartition,nEdges);
                break;
            }
            case MAX_MODULARITY_ASYMPT:
            {
                curQual = getModularityAsympt(curPartition,nVertices,nEdges);
                break;
            }
            }
            if ( curQual > qual )
            {
                //cout << "<e1,e2>=(" << e.first << "," << e.second << ")\tS=" << qual << endl;
                qual = curQual;
            }
            else
            {
                curPartition.undo();
            }
        }
        ++iteration;
    }

    curPartition.sortBySize();
    this->partition = curPartition;
    return pair<double,DisjointSet>(qual,curPartition);
}



/**
 * @brief Fagso::run
 * Run the fagso algorithm and keep the partition with highest Surprise
 */
void Fagso::run()
{
    FILE_LOG(logINFO) << "Running Fagso: maximizing " << costFunctionNames[params.costFunc];
    double quality=0;
    for (size_t i=0; i<params.repetitions; i++)
    {
        FILE_LOG(logDEBUG1) << "Running Fagso iteration " << i;
        pair<double, DisjointSet> sol = runFagsoSingleIter();
        FILE_LOG(logDEBUG1) << "Current quality " << sol.first;
        if (sol.first > quality)
        {
            quality = sol.first;
            this->partition = sol.second;
            if (params.saveSolution)
            {
                FILE_LOG(logDEBUG1) << "Solution saved to = " << savestring(params,quality);
                this->partition.save(savestring(params,quality));
            }
        }
        FILE_LOG(logDEBUG1) << "Best S=" << quality;
    }

    if (params.printsolution)
        this->partition.print(cout);

    this->finalSurprise = getSurprise(partition,nVertices,nEdges);
    this->finalSignificance  = getSignificance(partition,nVertices,nEdges);
    this->finalAsymptoticSurprise = getAsymptoticSurprise(partition,nVertices,nEdges);
    this->finalModularity = getModularity(partition,nEdges);
    this->finalAsymptoticModularity = getModularityAsympt(partition,nVertices,nEdges);

    FILE_LOG(logINFO) << "Final Surprise=" << finalSurprise;
    //FILE_LOG(logINFO) << "Final Significance=" << finalSignificance;
    //FILE_LOG(logINFO) << "Final AsymptoticSurprise=" << finalAsymptoticSurprise;
    //FILE_LOG(logINFO) << "Final Modularity=" << finalModularity;
    //FILE_LOG(logINFO) << "Final AsymptoticModularity=" << finalAsymptoticModularity;
    if (params.saveSolution)
    {
        FILE_LOG(logINFO) << "Solution saved to = " << savestring(params,quality);
    }
}

/**
 * @brief Fagso::getFinalSignificance
 * @return
 */
double Fagso::getFinalSignificance()
{
    return finalSignificance;
}

/**
 * @brief Fagso::getFinalSurprise
 * @return
 */
double Fagso::getFinalSurprise()
{
    return finalSurprise;
}

/**
 * @brief Fagso::getFinalAsymptoticSurprise
 * @return
 */
double Fagso::getFinalAsymptoticSurprise()
{
    return finalAsymptoticSurprise;
}

/**
 * @brief Fagso::getFinalModularity
 * @return
 */
double Fagso::getFinalModularity()
{
    return finalModularity;
}

/**
 * @brief Fagso::getFinalQuality
 * @return
 */
double Fagso::getFinalQuality()
{
    double finalQuality;
    switch (params.costFunc)
    {
    case MAX_SURPRISE_DISCRETE:
    {
        finalQuality = getFinalSurprise();
        break;
    }
    case MAX_SIGNIFICANCE_ASYMPT:
    {
        finalQuality = getFinalSignificance();
        break;
    }
    case MAX_SURPRISE_ASYMPT:
    {
        finalQuality = getFinalAsymptoticSurprise();
        break;
    }
    case MAX_MODULARITY:
    {
        finalQuality = getFinalModularity();
        break;
    }
    default:
    {
        finalQuality = -1;
    }
    }
    return finalQuality;
}
