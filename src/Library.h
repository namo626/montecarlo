#ifndef LIBRARY_H
#define LIBRARY_H

typedef struct Result Result;

// run the monte carlo simulation with the given parameters
Result* runSim(int trials, int size_estimate, int planners, int labor);

// extract arrays from the result object
double* totalTime(Result* result);
double* totalCost(Result* result);

// print the histogram from a general double array
void printHist(double* results, int n, int binSize);

// print the whole double array
void printArr(double* arr, int length);

// turn debugging on or off
void debugMode(char* str);

#endif
