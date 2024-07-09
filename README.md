## PyPHX
PyPHX is a python wrapper for the Active Silicon Phoenix board series. The code 
contains a visual studio solution with  written in c that when compiled into dll contains functions 
to grab images either continuously or sequentially. Buffers are managed 
and transferred to python. 

## Features
- **Frame Grabber Control**: Includes utilities for handling and analyzing 
- **Visualization**: Provides easy-to-use functions for creating plots 
with `matplotlib`, or with the active silicon display libraries.


## Installation 
Install the PHX_API SDK and configure environmental variables to point to 
the root installation directory. 

Open the *.sln file located in the `c` directory. build the solution. The output 
dll will be placed into the `build` folder. 

The package contains two solutions. One will run a live image view, and the other is an interface to the Grabber Library

Install the module with `pip install PyPHX`
