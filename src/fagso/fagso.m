% This file is part of FAGSO, a program to find network partitions
%
%  Copyright (C) 2014-2015 Carlo Nicolini <carlo.nicolini@iit.it>

%  FAGSO is free software; you can redistribute it and/or
%  modify it under the terms of the GNU Lesser General Public
%  License as published by the Free Software Foundation; either
%  version 3 of the License, or (at your option) any later version.

%  Alternatively, you can redistribute it and/or
%  modify it under the terms of the GNU General Public License as
%  published by the Free Software Foundation; either version 2 of
%  the License, or (at your option) any later version.

%  FAGSO is distributed in the hope that it will be useful, but WITHOUT ANY
%  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
%  FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
%  GNU General Public License for more details.

%  You should have received a copy of the GNU Lesser General Public
%  License and a copy of the GNU General Public License along with
%  FAGSO. If not, see <http://www.gnu.org/licenses/>.

function [ci, varargout] = fagso(A, varargin)
% FAGSO: Fast Agglomerative Surprise/Significance optimization
% Example:
% % Generate a nxn symmetric matrix of random {1,0}
% A = random(100) > 0.05
% A = (A+A')/2
% m, surp, sign = fagso(A, sort_method=8, repetitions=10)
% 
% Args: 
% A: Adjacency matrix of the graph, symmetric, binary.
%    
% varargin:
% sort_method: sorting method for the edges, (default 0:Jaccard):
%     0: Jaccard,
%     1: AdamicAdar,
%     2: AverageDegree,
%     3: CommonNeighbors,
%     4: HubDepressed ,
%     5: HubPromoted,
%     6: LeichtHolmeNewman,
%     7: PreferentialAttachment,
%     8: Random,
%     8: ResourceAllocation,
%     9: Salton,
%     10: EdgeLength,
%     11: Sorensen,
%     12: JaccardNormalized,
%     13: Cosine
%
% repetitions: number of repetitions of FAGSO (different results for randomized ranking of edges), (default 1).
% kfold: number of folds within randomization is carried, can help yielding better partitions, (default 1)
% verbosity: level of verbosity of output:  ERROR=0, WARNING=1, INFO=2, DEBUG=3, DEBUG1=4, DEBUG2=5, DEBUG3=6, DEBUG4=7 (default 0)
% seed: random seed for randomization (integer value)
% first_edge: index of the edge to be put as first edge to start agglomeration (default -1)
% quality_function: index of quality function to optimize with FAGSO:
%    0: Surprise (discrete definition)
%    1: Significance (asymptotical)
%    2: Asymptotic surprise (faster to compute on large graphs)
%    3: Asymptotic modularity (experimental!)
% Out:
% membership: a list of vertices community membership
% surprise: the value of Surprise for the current partition
% significance: the value of Significance for the current partition

    numvarargs = length(varargin);

    if numvarargs > 5
        error('FAGSO: too many input arguments', ...
             'FAGSO requires at most 4 optional inputs');
    end

    % set defaults for optional inputs
    optargs = {0 1 1 @magic};
    optargs(1:numvarargs) = varargin;

    % Skip any new inputs if they are empty
    newVals = cellfun(@(x) ~isempty(x), varargin);
    % now put these defaults into the valuesToUse cell array, 
    % and overwrite the ones specified in varargin.
    optargs(newVals) = varargin(newVals);
    
    [sortmethod, rep, kfold] = optargs{:};
    sm = 0;
    switch sortmethod
        case 'jaccard'
            sm = 0;
        case 'adamicadar'
            sm = 1;
        case 'avgdeg'
                sm = 2;
        case 'comneigh'
                sm = 3;
        case 'hubdepr '
                sm = 4;
        case 'hubprom'
                sm = 5;
        case 'leichtholme'
                sm = 6;
        case 'prefattach'
                sm = 7;
        case 'random'
                sm = 8;
        case 'resalloc'
                sm = 9;
        case 'salton'
                sm = 10;
        case 'edgeLength'
                sm = 11;
        case 'sorensen'
                sm = 12;
        case 'jaccardnorm'
                sm = 13;
        case 'cosine'
                sm = 14;
    end
    if isoctave()
       ci = fagso_oct(A,sm,rep,kfold);
    else
        ci = fagso_mx(A,sm,rep,kfold);
    end
    
function res = isoctave()
	res = exist('OCTAVE_VERSION', 'builtin') ~= 0;