# FAGSO "Fast Agglomerative Surprise Optimization"

This is the implementation of FAGSO, the algorithm described in "Modular structure of brain functional networks: breaking the resolution limit by Surprise" by Nicolini C. and Bifone A.
FAGSO comes as a C++ library but wrappers for Matlab/Octave and Python are available. Instructions on how to build FAGSO as well as its Matlab, Octave and Python wrappers are described below.
For further problems, don't hesitate to contact me.

## Requirements
FAGSO needs CMake ( www.cmake.org ) to generate the cross-plattform Makefile and a C/C++ compiler to make the executable.
On Debian-based systems this should suffice:

    $> sudo apt-get install build-essential cmake

On OSX you'd need of a working installation of XCode and clang compiler, you can then downloade and install CMake directly from their website.

### Supported compilers
FAGSO is tested on Ubuntu >= 14.04 with **gcc/clang** compiler and OSX Yosemite with **clang** compiler.

## Compilation
To compile FAGSO open a terminal window, then issue this commands:

    $> git clone https:/github.com/CarloNicolini/fagso.git
    $> cd fagso
    $> mkdir build
    $> cd build
    $> cmake ..
    $> make all

This will compile the FAGSO libraries and a simple executable `fagso_main` that can be run on command line.

### MATLAB/OCTAVE WRAPPERS
If you want to compile `fagso_mx` mex function, you need to enable the `MATLAB_SUPPORT` flag at compile time:

    $> cmake -DMATLAB_SUPPORT=True ..

or equivalently for Octave:

    $> cmake -DOCTAVE_SUPPORT=True ..

and then compile the source with `make`:

    $> make all

### PYTHON WRAPPERS
FAGSO libraries can be exposed as Python module through cython.
To compile the `pyfagso` python module you need `cython >= 0.17`:

On Debian-based Linux you can install cython via the repositories:

    $> sudo apt-get install python-cython

and then enable the python support of FAGSO:

    $> cmake -DPYTHON_SUPPORT=True ..
    $> make all

CMake will show specific errors if some missing package is requested for the compilation. Just be sure always to be up-to-date with **numpy** and **cython** packages:

    $> sudo pip install -U numpy cython


Once `pyfagso` is compiled start a python console and import pyfagso as a normal Python module

    $> python
    >>> import pyfagso

On OSX I strongly suggest to obtain cython through `pip` by first installing `pip` using `homebrew`. 

# Windows support:
Despite everything should be ready to be ported happily in Windows, I don't have time to let the code compile smoothly on Windows. If you want to join me in porting FAGSO for Windows let me know.

# Using FAGSO
## Using FAGSO as a standalone executable
FAGSO can be used as a standalone executable. First compile FAGSO as described before, to recap:

    $> git clone https://github.com/carlonicolini/fagso.git
    $> cd fagso
    $> mkdir build
    $> cd build
    $> cmake ..
    $> make

This will generate the `fagso_main` executable. Run it to see a short guide:

    $> ./fagso_main
       Usage: fagso_run [options] graph_file [output_suffix]
       options:
       -s [method]: Edges sorting method, default=0 (Jaccard Index) 
          0 Jaccard
          1 AdamicAdar
          2 AverageDegree
          3 CommonNeighbors
          4 HubDepressed
          5 HubPromoted
          6 LeichtHolmeNewman
          7 PreferentialAttachment
          8 Random
          9 ResourceAllocation
          10 Salton
          11 Sorensen
          12 Jaccard Normalized
          13 Cosine Similarity
       -V [report_level] ERROR=0, WARNING=1, INFO=2, DEBUG=3, DEBUG1=4, DEBUG2=5, DEBUG3=6, DEBUG4=7
       -S [seed] specify the random seed, default time(0)
       -m [bool] wheter to maximize discrete Surprise (0), Significance (1), asymptotic Surprise (2) or asymptotic Modularity (3), default (0)
       -b [bool] wheter to start with initial random cluster or every node in its community
       -r [repetitions], number of repetitions of FAGSO, default=1
       -v [kfold] Number of kfold division for randomization, default=1
       -e [firstedgetoswap] Index of the edge to be put as first edge to start agglomeration
       -o [output folder] Folder where to put the result. If not specified is the same as the input file.
       -w [save solution] 1 to save solution, 0 not to save, (default 0).
       -p [print solution]
       -P [shuffle percentage] shuffle percentage, in [0,1] interval, to make search more exhaustive

FAGSO accepts as input, networks in binary adjacency matrix format.
You can try to run FAGSO on an example graph with verbosity level set to 7 (maximum level)

    $> ./fagso_main -V 7 -p 1 ../data/karate.adj

This will print out (`-p 1` command) community membership of every vertex (`=== Memberships ===`) and vertices (`=== Groups ====`) of every community.


## Using FAGSO as Matlab/Octave mex file

If you prefer to use FAGSO as a Matlab mex file, follow the instructions on how to compile it for Matlab (already described). Then start a Matlab/Octave session: 

    $> matlab
    (matlab is starting...)
    >> 

At this point try to load a network as a binary symmetric adjacency matrix. We load the same `karate.adj` matrix:

    >> K=load('../data/karate.adj');
    >> [memb, surp] = fagso_mx(K);

The array `memb` contains the integer indices of community membership of vertices. The `surp` is the value of Surprise.

For **Octave** the syntax is similar:

    >> K=load('../data/karate.adj');
    >> [memb, surp] = fagso_oct(K);


# FAQ AND TROUBLESHOOTING
---

## Linking libstdc++.so.6 problem

I can compile FAGSO for MATLAB but after calling `fagso_mx`, MATLAB prompts me with the following error message:

```
Invalid MEX-file '~/fagso_mx.mexa64': /usr/local/MATLAB/R2015a/bin/glnxa64/../../sys/os/glnxa64/libstdc++.so.6: version `GLIBCXX_3.4.21' not found (required by ...
```

This problem means that the `libstdc++.so.6` inside the Matlab library folder is pointing to a version of `libstdc++` older than the system one, usually stored in `/usr/lib/x86_64` folder.

To solve the issue you need to redirect the symbolic links in the MATLAB folder to the systemwise `libstdc++`. Hereafter we assume the MATLAB folder to be `/usr/local/MATLAB/R2015a` and the system to be some Linux variant.

Two of the symlinks for libraries need to be changed:

```
$> cd /usr/local/MATLAB/R2015a/sys/os/glnxa64
$> ls -l
```

The sym links for libstdc++.so.6 and libgfortran.so.3 should point to versions in /usr/lib, not local ones.


Before changing this libraries, first make sure `g++-4.4` and `libgfortran3`are installed :

```
$> sudo apt-get install g++-4.4 libgfortran3
```

Now, modify the symlinks:

```
$> sudo ln -fs /usr/lib/x86_64-linux-gnu/libgfortran.so.3.0.0 libgfortran.so.3
$> sudo ln -fs /usr/lib/gcc/x86_64-linux-gnu/4.4/libstdc++.so libstdc++.so.6
```

This command makes the `libstdc++.so.6` point to the `g++-4.4` `libstdc++` library.

For other informations take a look at http://dovgalecs.com/blog/matlab-glibcxx_3-4-11-not-found/ or https://github.com/RobotLocomotion/drake/issues/960 which are very similar problems.

### Compiler making wrong assumptions on mex.h header:

If you want to have both MATLAB and OCTAVE mex files, I suggest, in order to avoid problems with the wrong `mex.h` import to first build with Matlab support, then copy the resulting `fagso_mx` in some folder, then clean the repository and recompile with `-DOCTAVE_SUPPORT`. It may happen indeed that the compilers import the `mex.h` from the Octave devel libraries when compiling the MATLAB libraries or viceversa. Compiling them separately solves the issue.
