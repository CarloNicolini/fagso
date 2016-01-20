cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Fagso)

set(FAGSO_SRCS
    AsymptoticEntropy.cpp
    AsymptoticModularity.cpp
    AsymptoticSignificance.cpp
    AsymptoticSurprise.cpp
    DisjointSet.cpp
    Fagso.cpp
    Graph.cpp
    KLDivergence.cpp
    SimilarityIndices.cpp
    Surprise.cpp
    Timer.cpp
    Util.cpp
)

set(FAGSO_HDRS
    AsymptoticEntropy.h
    AsymptoticModularity.h
    AsymptoticSignificance.h
    AsymptoticSurprise.h
    DisjointSet.h
    Fagso.h
    FagsoParams.h
    FileLogger.h
    Graph.h
    KLDivergence.h
    SimilarityIndices.h
    Surprise.h
    Timer.h
    Util.h
)

add_library(FAGSO SHARED ${FAGSO_SRCS} ${FAGSO_HDRS})

if(MATLAB_SUPPORT)
        add_mex(fagso_mx fagso_mx.cpp ${FAGSO_SRCS})
        #target_link_libraries(fagso_mx FAGSO ${MATLAB_LIBRARIES})
endif(MATLAB_SUPPORT)

if(OCTAVE_SUPPORT)
    #octave_add_oct(fagso_oct SOURCES fagso_mx.cpp LINK_LIBRARIES FAGSO EXTENSION "mex")
endif(OCTAVE_SUPPORT)

if(PYTHON_SUPPORT)
    set_source_files_properties(pyfagso.pyx PROPERTIES CYTHON_IS_CXX 1 )
    message(STATUS ${PYTHON_INCLUDE_DIR})
    # put cython wrapper and the implementation of the classes used (NOT THE CXX!)
    cython_add_module(pyfagso pyfagso.pyx)
    target_link_libraries(pyfagso FAGSO)
endif(PYTHON_SUPPORT)

add_executable(fagso_main fagso_main.cpp)
target_link_libraries(fagso_main FAGSO)
