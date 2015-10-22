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

#include "FagsoParams.h"
#include "Fagso.h"
#include "FileLogger.h"

/**
 * @brief exit_with_help
 */
void exit_with_help()
{
    std::printf(
                "Usage: fagso_run [options] graph_file [output_suffix]\n"
                "options:\n"
                "-s [method]: Edges sorting method, default=0 (Jaccard Index) \n"
                "   0 Jaccard\n"
                "   1 AdamicAdar\n"
                "   2 AverageDegree\n"
                "   3 CommonNeighbors\n"
                "   4 HubDepressed\n"
                "   5 HubPromoted\n"
                "   6 LeichtHolmeNewman\n"
                "   7 PreferentialAttachment\n"
                "   8 Random\n"
                "   9 ResourceAllocation\n"
                "   10 Salton\n"
                "   11 Sorensen\n"
                "   12 Jaccard Normalized\n"
                "   13 Cosine Similarity\n"
                "-V [report_level] ERROR=0, WARNING=1, INFO=2, DEBUG=3, DEBUG1=4, DEBUG2=5, DEBUG3=6, DEBUG4=7\n"
                "-S [seed] specify the random seed, default time(0)\n"
                "-m [bool] wheter to maximize discrete Surprise (0), Significance (1), asymptotic Surprise (2) or asymptotic Modularity (3), default (0)\n"
                "-b [bool] wheter to start with initial random cluster or every node in its community\n"
                "-r [repetitions], number of repetitions of FAGSO, default=1\n"
                "-v [kfold] Number of kfold division for randomization, default=1\n"
                "-e [firstedgetoswap] Index of the edge to be put as first edge to start agglomeration\n"
                "-o [output folder] Folder where to put the result. If not specified is the same as the input file.\n"
                "-w [save solution] 1 to save solution, 0 not to save, (default 0).\n"
                "-p [print solution]\n"
                "-P [shuffle percentage] shuffle percentage, in [0,1] interval, to make search more exhaustive\n"
                "\n"
                );
    exit(1);
}

/**
 * @brief parse_command_line
 * @param argc
 * @param argv
 * @param input_file_name
 */
Parameters parse_command_line(int argc, char **argv)
{
    //void (*print_func)(const char*) = NULL;   // default printing to stdout
    // parse options
    Parameters params;
    params.sortmethod = SortJaccard;
    params.kfold = 1;
    params.repetitions=1;
    params.firstEdge=-1;
    params.printsolution=false;
    params.initialRandomize=false;
    params.saveSolution=false;
    params.costFunc = MAX_SURPRISE_DISCRETE;
    params.seed = -1;
    params.verbosity = 0;
    params.shufflePercentage=0.0;

    int i=1;
    for(i=1; i<argc; i++)
    {
        if(argv[i][0] != '-')
            break;
        if(++i>=argc)
            exit_with_help();
        switch(argv[i-1][1])
        {
        case 'V':
        {
            params.verbosity = atoi(argv[i]);
            if (params.verbosity>7)
                params.verbosity=7;
            FILE_LOG(logDEBUG) << "Verbosity level " << static_cast<TLogLevel>(params.verbosity);
            FILELog::ReportingLevel() =  static_cast<TLogLevel>(params.verbosity);
            break;
        }
        case 's':
        {
            FILE_LOG(logDEBUG) << "Sorting by " << sortMethodNames[atoi(argv[i])];
            params.sortmethod = static_cast<SortMethod>(atoi(argv[i]));
            break;
        }
        case 'S':
        {
            params.seed = atoi(argv[i]);
            FILE_LOG(logDEBUG) << "Setting specific random seed " << params.seed;
            break;
        }
        case 'm':
        {
            switch (atoi(argv[i]))
            {
            case 0:
            {
                FILE_LOG(logDEBUG) << "Optimizing " << costFunctionNames[0];
                params.costFunc = MAX_SURPRISE_DISCRETE;
                break;
            }
            case 1:
            {
                FILE_LOG(logDEBUG) << "Optimizing " << costFunctionNames[1];
                params.costFunc = MAX_SIGNIFICANCE_ASYMPT;
                break;
            }
            case 2:
            {
                FILE_LOG(logDEBUG) << "Optimizing " << costFunctionNames[2];
                params.costFunc = MAX_SURPRISE_ASYMPT;
                break;
            }
            case 3:
            {
                FILE_LOG(logDEBUG) << "Optimizing " << costFunctionNames[3];
                params.costFunc = MAX_MODULARITY;
                break;
            }
            default:
            {
                FILE_LOG(logERROR) << "Parameter m can be 0,1";
                exit_with_help();
            }
            }
            break;
        }
        case 'b':
        {
            FILE_LOG(logDEBUG) << "Initial randomization of community structure";
            params.initialRandomize = bool(atoi(argv[i]));
            break;
        }
        case 'v':
        {
            params.kfold = atoi(argv[i]);
            FILE_LOG(logINFO) << "Setting k-fold randomization to " << params.kfold;
            break;
        }
        case 'r':
        {
            params.repetitions = atoi(argv[i]);
            FILE_LOG(logINFO) << "Setting repetitions to " << params.repetitions;
            break;
        }
        case 'e':
        {
            params.firstEdge = atoi(argv[i]);
            FILE_LOG(logDEBUG) << "Setting first edge to " << params.firstEdge;
            break;
        }
        case 'p':
        {
            params.printsolution = true;
            break;
        }
        case 'o':
        {
            params.outputFolder = std::string(argv[i]);
            FILE_LOG(logDEBUG) << "Setting output folder to " << params.outputFolder;
            break;
        }
        case 'c':
        {
            params.coordinateFileName = std::string(argv[i]);
            FILE_LOG(logDEBUG) << "Setting coordinate file " << params.coordinateFileName;
            break;
        }
        case 'P':
        {
            params.shufflePercentage = atof(argv[i]);
            if (params.shufflePercentage>1 || params.shufflePercentage<0)
                FILE_LOG(logERROR) << "Must provide shuffle percentage in [0.0,1.0]" ;
            break;
        }
        default:
            FILE_LOG(logERROR) << "Unknown option: " << argv[i-1][1] ;
            exit_with_help();
        }
    }

    // Determine filenames
    if(i>=argc)
        exit_with_help();

    char input[1024];
    strcpy(input, argv[i]);
    params.inputGraphFileName = std::string(input);

    std::ifstream is(input);
    if (!is.good())
    {
        cout << std::string("File \"" + params.inputGraphFileName + "\" not found") << endl;
        exit_with_help();
    }
    return params;
}


int main(int argc, char *argv[])
{
    Parameters params = parse_command_line(argc,argv);
    Timer timer;
    if (params.seed==-1)
        srand(timer.getElapsedTimeInMicroSec());
    else
        srand(params.seed);

    GraphAdj A;
    if (has_suffix(params.inputGraphFileName,".adj") || has_suffix(params.inputGraphFileName,".csv"))
        A = readAdjacencyMatrix(std::string(params.inputGraphFileName));
    else if (has_suffix(params.inputGraphFileName,".dimacs"))
        A = readDimacs(params.inputGraphFileName);
    else
        throw std::runtime_error("Error: non supported format. Use \".adj\" or \".csv\" for adjacency matrices or \".dimacs\" for adjacency lists");

    if ( A.cols()==0 || A.rows()==0 )
    {
        cerr << "Error: Graph has no node or edge: |V|,|E|=" << A.cols() << "," << A.sum()/2 << endl;
        throw std::runtime_error("Invalid graph");
        return 1;
    }

    Fagso fagso(A,params);
    fagso.run();

    return 0;
}
