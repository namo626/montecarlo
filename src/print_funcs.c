/*
 * print_funcs.c
 *
 *  Created on: 1 dec. 2018
 *      Author: axel_
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "rand_dist.h"
#include "Library.h"
#include "print_funcs.h"

void read_file(char* configfile) {
	printf("Starting to read file...\n");
	if (configfile == NULL) {
		return;
	}
	FILE* ifp;
	ifp = fopen(configfile, "r");
	if (ifp == NULL) {
		printf("Could not open file");
		return;
	}
	char c[1000];
	int num = 1;
	while (fgets(c, sizeof c, ifp) != NULL) {
		printf("Experiment: %d\n", num);
		printf("Input values: %s\n", c);
		char *split = strtok(c, " ");
		int i = 0;
		int size_estimate, planners, labors, trials;
		char* debug;
		char* outfile;
		while (split != NULL) {
			if (i == 0) {
				size_estimate = atoi(split);
				printf("Size: %d\n", size_estimate);
			} else if (i == 1) {
				planners = atoi(split);
				printf("Planners: %d\n", planners);
			} else if (i == 2) {
				labors = atoi(split);
				printf("Labors: %d\n", labors);
			} else if (i == 3) {
				trials = atoi(split);
				printf("Trials: %d\n", trials);
			} else if (i == 4) {
				debug = split;
				printf("Debug mode: %s\n", split);
			} else if (i == 5) {
				outfile = split;
				printf("Output file: %s\n", split);
			} else {
				printf("Wrong number of arguments in input file.\n");
				exit(1);
			}
			i++;
			split = strtok(NULL, " ");
		}
		Result* results = runSim(trials, size_estimate, planners, labors, debug);
		if (results == NULL) {
			printf("No results were generated\n");
			exit(1);
		}
		print_output(results, trials, outfile);
		num++;
	}
	fclose(ifp);
}

void print_output(Result* results, int trials, char* outfile) {
	printf("Writing to file...\n");
	FILE *ofp;
	ofp = fopen(outfile, "w");
	double* time_taken = totalTime(results);
	double* cost_involved = totalCost(results);

	printf("Time distribution:\n");
	printHist(time_taken, trials, 10);
	printf("\n");
	printf("Cumulative time distribution:\n");
	printCumulativeHist(time_taken, trials, 10);
	printf("Expected value  Standard Dev\n");
	double expected_time = expectation(time_taken, trials);
	double dev_time = deviation(time_taken, trials);
	printf("%.2f         %.2f\n", expected_time, dev_time);

	printf("\n");
	printf("Cost distribution:\n");
	printHist(cost_involved, trials, 200);
	printf("\n");
	printf("Cumulative cost distribution:\n");
	printCumulativeHist(cost_involved, trials, 200);
	printf("Expected value  Standard Dev\n");
	fprintf(ofp, "Expected value  Standard Dev\n");
	double expected_cost = expectation(cost_involved, trials);
	double dev_cost = deviation(cost_involved, trials);
	printf("%.2f         %.2f\n", expected_cost, dev_cost);
	fprintf(ofp, "%.2f         %.2f\n", expected_cost, dev_cost);

	fclose(ofp);
	printf("Done writing to file\n\n");
}
