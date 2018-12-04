#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "rand_dist.h"
#include "Library.h"
#include "print_funcs.h"

void print_cumulative_hist(double *results, int n, int binSize, FILE *ofp);
void print_hist_to_file(double* arr, int n, int binSize, FILE *ofp);

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
				if (outfile[strlen(outfile) - 1] == '\n') {
					outfile[strlen(outfile) - 1] = '\0';
				}
				printf("Output file: %s\n", split);
			} else {
				printf("Wrong number of arguments in input file.\n");
				exit(1);
			}
			i++;
			split = strtok(NULL, " ");
		}
		Result* results = runSim(trials, size_estimate, planners, labors,
				debug);
		if (results == NULL) {
			printf("No results were generated\n");
			exit(1);
		}
		print_output(results, trials, outfile);
		num++;
	}
	fclose(ifp);
}

double max(double* arr, int length) {
	if (arr == NULL) {
		return 0;
	}

	double max = arr[0];

	for (int i = 1; i < length; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}

	return max;
}

void print_output(Result* results, int trials, char* outfile) {
	printf("Writing to file...\n");
	FILE *ofp;
	ofp = fopen(outfile, "w");
	double* time_taken = totalTime(results);
	double* cost_involved = totalCost(results);

	printf("Time distribution:\n");
	fprintf(ofp, "Time distribution:\n");
	printHist(time_taken, trials, 10);
	print_hist_to_file(time_taken, trials, 10, ofp);
	printf("\n");
	printf("Cumulative time distribution:\n");
	fprintf(ofp, "\n\nCumulative time distribution:\n");
	print_cumulative_hist(time_taken, trials, 10, ofp);
	printf("Expected value  Standard Dev\n");
	fprintf(ofp, "\nExpected value  Standard Dev\n");
	double expected_time = expectation(time_taken, trials);
	double dev_time = deviation(time_taken, trials);
	printf("%.2f         %.2f\n", expected_time, dev_time);
	fprintf(ofp, "%.2f         %.2f\n", expected_time, dev_time);

	printf("\n");
	printf("Cost distribution:\n");
	printHist(cost_involved, trials, 200);
	fprintf(ofp, "\n\nCost distribution:\n");
	print_hist_to_file(cost_involved, trials, 200, ofp);
	printf("\n");
	printf("Cumulative cost distribution:\n");
	fprintf(ofp, "\n\nCumulative cost distribution:\n");
	print_cumulative_hist(cost_involved, trials, 200, ofp);
	printf("Expected value  Standard Dev\n");
	fprintf(ofp, "\nExpected value  Standard Dev\n");
	double expected_cost = expectation(cost_involved, trials);
	double dev_cost = deviation(cost_involved, trials);
	printf("%.2f         %.2f\n", expected_cost, dev_cost);
	fprintf(ofp, "%.2f         %.2f\n", expected_cost, dev_cost);

	fclose(ofp);
	printf("Done writing to file\n\n");
}

void print_cumulative_hist(double *results, int n, int binSize, FILE* ofp) {
	// print the cumulative histogram of a given result array (1D)
	if (results == NULL || n == 0) {
		printf("No results given\n");
		return;
	}

	if (binSize < 1) {
		printf("Bin width must be at least 1\n");
		return;
	}

	int upper = binSize;

	double range = results[0];

	for (int i = 1; i < n; i++) {
		if (results[i] > range) {
			range = results[i];
		}
	}

	int steps = 1 + (range / binSize);

	for (int i = 0; i < steps; i++) {
		printf("%d:", upper);
		fprintf(ofp, "%d:", upper);
		// sum the amount in this bin
		int sum = 0;
		for (int i = 0; i < n; i++) {
			if (results[i] <= upper) {
				sum = sum + 1;
			}
		}
		printf("  %d ", sum);
		fprintf(ofp, "  %d ", sum);

		for (int j = 0; j < (sum / 5); j++) {
			printf("x ");
			fprintf(ofp, "x ");
		}
		printf("\n");
		fprintf(ofp, "\n");

		upper = upper + binSize;
	}

}

void print_hist_to_file(double* arr, int n, int binSize, FILE *ofp) {
	if (arr == NULL || n == 0) {
		printf("No array given\n");
		return;
	}

	if (binSize < 1) {
		printf("Bin width must be at least 1\n");
		return;
	}

	int lower = 0;
	int upper = binSize;
	double range = max(arr, n);
	int steps = 1 + (range / binSize);

	for (int i = 0; i < steps; i++) {
		fprintf(ofp, "%d", upper);

		// sum the amount in this bin
		int sum = 0;
		for (int i = 0; i < n; i++) {
			if (lower < arr[i] && arr[i] <= upper) {
				sum = sum + 1;
			}
		}
		fprintf(ofp, " %d\n", sum);

		lower = upper;
		upper = upper + binSize;
	}
}

