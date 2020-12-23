# SMatrix

A very (very) simple matrix library with basic operations.

This is some old code that should compile with C++2003 and maybe earlier.
Matrices are backed by a simple vector.
This is in no way intended for high performance computing. It's the bare
minimum for the occasional use in embedded environment, when small matrices 
are needed (typically less than 20x20) and you don't want to embed a full fledged 
matrix library due to space constraints.
Most operations are done on the stack so it should be reasonably fast.
The code is simple enough to be easily extended as needed.
This has been successfully been used in hard real time production environment to replace
Fortran code.

Published under MIT License.
