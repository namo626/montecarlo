#ifndef LIBRARY_H
#define LIBRARY_H

/* Container of result */
typedef struct Result Result;

/*
 * Run the monte carlo simulation
 * param trials number of iterations, min is 1
 * param size_estimate the size of the project between 1 and 10
 * param planners number of planners for the project
 * param labor number of construction workers
 * param debugMode turn debugging on/off ("TRUE"/"FALSE")
 * return the result object
 */
Result* runSim(int trials, int size_estimate, int planners, int labor, char* debugMode);

/*
 * Extract time array from the result object
 * param result the result object
 * return the array of doubles containing time spent for each iteration
 */
double* totalTime(Result* result);

/*
 * Extract cost array from the result object
 * param result the result object
 * return the array of doubles containing cost spent for each iteration
 */
double* totalCost(Result* result);

/*
 * Print the histogram from an generic array of doubles
 * param arr the array of doubles
 * param n length of the array
 * param binSize bin width of the histogram
 */
void printHist(double* arr, int n, int binSize);

/*
 * Print an array of doubles vertically
 * param arr the array of doubles
 * param length length of array
 */
void printArr(double* arr, int length);

/*
 * Calculate the mean (expectation) of a generic array of doubles
 * param arr the array of doubles
 * param length length of array
 * return mean value
 */
double expectation(double* arr, int length);

/*
 * Calculate the standard deviation of a generic array of doubles
 * param arr the array of doubles
 * param length length of array
 * return standard deviation
 */
double deviation(double* arr, int length);

#endif
