Author          : Axel Berglund and Chayanon Wichitrnithed
Created         : December 04, 2018
Last Modified   : December 04, 2018

Affiliation          : Georgia Institute of Technology


Description
-------------

Program to run a Monte Carlo simulation for a construction project with several stages.
Use an input file where each line has the following format:
SIZE PLANNERS WORKERS TRIALS DEBUG_MODE OUTPUT_FILE
The program will then run a simulation using the supplied values.
The output will be written to a file with the name specified as OUTPUT_FILE


Installation
------------

To install, simply run

    gcc -o main Main.c Library.c rand_dist.c print_funcs.c -std=c99 -lm

Note: The -std=c99 flag is required on some C compilers
to allow variable declarations at arbitrary locations in
a function.

Execution
-----------

Assuming your executable is called "main", run it using

    ./main input.txt
    
Where input.txt is the name of the configuration/input file.


