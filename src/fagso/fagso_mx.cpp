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

#include <iostream>
#include "Fagso.h"

#ifdef MATLAB_SUPPORT
#include "/usr/local/MATLAB/R2015a/extern/include/mex.h"
#elif OCTAVE_SUPPORT
#include "/usr/include/octave-4.0.0/octave/mex.h"
#endif

#define IS_ADJACENCY_MATRIX(P) (!mxIsComplex(P) && mxGetNumberOfDimensions(P) == 2 && !mxIsSparse(P) && (mxIsDouble(P) || mxIsUint8(P) || mxIsUint8(P) || mxIsLogical(P)))

void mexFunction(int nOutputArgs, mxArray *outputArgs[], int nInputArgs, const mxArray * inputArgs[])
{
    if( nInputArgs > 5 ) // Check the number of arguments
        mexErrMsgTxt("Too many input arguments.");
    if (nOutputArgs > 2)
        mexErrMsgTxt("Too many output arguments.");

    if(!IS_ADJACENCY_MATRIX(inputArgs[0])) // Check A
        mexErrMsgTxt("Input matrix must be a real 2D square adjacency matrix .");

    int M = mxGetM(inputArgs[0]);
    int N = mxGetN(inputArgs[0]);
    if (M!=N)
    {
        mexErrMsgTxt("Input matrix must be a real 2D square adjacency matrix.");
    }

    // Detect the sorting method
    Parameters params;
    params.sortmethod = SortJaccard;
    params.kfold = 1;
    params.repetitions = 1;
    params.verbosity = 0;

    if (nInputArgs>1)
        params.sortmethod =  static_cast<SortMethod>(mxGetScalar(inputArgs[1]));

    if (nInputArgs>2)
        params.kfold =  static_cast<SortMethod>(mxGetScalar(inputArgs[2]));
    if (nInputArgs>3)
        params.repetitions = static_cast<SortMethod>(mxGetScalar(inputArgs[3]));

    if (nInputArgs>4)
    {
        params.verbosity = static_cast<TLogLevel>(mxGetScalar(inputArgs[4]));
        FILELog::ReportingLevel() = static_cast<TLogLevel>(mxGetScalar(inputArgs[4]));
    }

    params.firstEdge=-1;

    params.printsolution=false;
    params.saveSolution = false;
    // Create the input matrix W as Eigen Matrix mapping the input matrix

    GraphAdj Adj = GraphAdj::Zero(N,N);
    for (int i=0; i<N*N; ++i)
        Adj.coeffRef(i)= (*(mxGetPr(inputArgs[0])+i)) > 0 ? 1.0: 0.0;
    //Adj = Adj.unaryExpr(std::ptr_fun(binarize)); // Important to binarize the matrix

    // Allocate space for the output matrix G
    Fagso fagso(Adj,params);
    fagso.run();

    // Returns the membership list
    std::vector<int> memb_int = fagso.getMembership();
    std::vector<double> memb_float(memb_int.begin(), memb_int.end());
    // Copy the membership list to the first output argument

    //std::vector<double> memb_float(M);
    outputArgs[0] = mxCreateDoubleMatrix(N,1,mxREAL);
    outputArgs[1] = mxCreateDoubleScalar(fagso.getFinalQuality());
    // Add the value of Surprise
    memcpy(mxGetPr(outputArgs[0]), memb_float.data(), sizeof(double)*N);
    return;
}
