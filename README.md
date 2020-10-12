# SMatrix

A very (very) simple matrix library with basic operations.

This is some old code that should compile with C++2003 and maybe earlier.
Matrices are backed by a simple vector.
This is in no way intended for high performance computing. It's the bare
minimum for the occasional use in embedded environment, when small matrices 
are needed (typically less than 20x20).
Most operations are done on the stack so it should be reasonably fast.
The code is simple enough to be easily extended as needed.
It has been successfully been used in real time production environment.

Published under MIT License.
