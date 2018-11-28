#ifndef LIBRARY_H
#define LIBRARY_H

typedef struct Result Result;

// run the monte carlo simulation with the given parameters
double** runSim(int trials, int size_estimate, int planners, int labor, int* materials);

// print the histogram
void printHist(double* results, int n, int binSize);

// turn debugging on or off
void debugMode(char* str);
