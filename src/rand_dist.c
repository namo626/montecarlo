/*
 * rand_dist.c
 *
 *  Created on: 16 nov. 2018
 *      Author: axel_
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rand_dist.h"

double normal_dist(double mean, double dev);
double uniform_dist(double min, double max);
void test_normal_dist();
void test_uniform_dist();

// int main() {
// 	srand(time(NULL));
// 	test_normal_dist();
// 	test_uniform_dist();
// }

void test_uniform_dist() {
	FILE* f = fopen("test_uniform.txt", "w");
	for (int i = 0; i < 50000; i++) {
		fprintf(f, "%f\n", uniform_dist(0, 1));
	}
	fclose(f);
	printf("Done\n");
}

//Test normal distribution
void test_normal_dist() {
	FILE* f = fopen("test_normal.txt", "w");
	for (int i = 0; i < 50000; i++) {
		fprintf(f, "%f\n", normal_dist(0, 1));
	}
	fclose(f);
	printf("Done\n");
}

/*
 * Generates a random number from a normal distribution with specified
 * mean and standard deviation using the Box-Mueller transform;
 * https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
 */
double normal_dist(double mean, double dev) {
	static int generate = 0;

	generate = !generate;

	static double z1;
	double u1, u2, z0;

	if (!generate) {
		return z1 * dev + mean;
	}
	u1 = ((double) rand() / (double) RAND_MAX);
	u2 = ((double) rand() / (double) RAND_MAX);
	//Since ln(0) is undefined, set z0 and z1 to 0 if u1 is 0
	if (u1 == 0) {
		z0 = 0;
		z1 = 0;
	} else {
		z0 = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
		z1 = sqrt(-2 * log(u1)) * sin(2 * M_PI * u2);
	}

	return z0 * dev + mean;
}

/*
 * Returns a random number from a uniform distribution with max and min value
 */
double uniform_dist(double min, double max) {
	double diff = max - min;
	double random_num = (diff / RAND_MAX) * rand() + min;
	return random_num;
}
