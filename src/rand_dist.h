#ifndef RAND_DIST_H
#define RAND_DIST_H

/*
 * Generates a random number from a uniform distribution.
 * param min the min value of the uniform distribution
 * param max the max value of the uniform distribution
 * return a random number from the uniform distribution
 */
double uniform_dist(double min, double max);

/*
 * Calculates a random number from a normal distribution3
 * using the Box-Muller transform.
 * param mean the mean of the normal distribution
 * param dev the standard deviation of the normal distribution
 * return a random number from the normal distribution
 */
double normal_dist(double mean, double dev);

#endif
