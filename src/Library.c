#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "rand_dist.h"
#include "Library.h"

// result container
struct Result {
  double* total_time;
  double* total_cost;
};

Result* mkResult(double* time, double* cost) {
  Result* result = malloc(sizeof(Result));
  if (result == NULL) {
    printf("Not enough memory\n");
    return NULL;
  }

  if (time == NULL || cost == NULL) {
    printf("No results given\n");
    return NULL;
  }

  result->total_cost = cost;
  result->total_time = time;
  return result;
}

double* totalTime(Result* result) {
  return result->total_time;
}

double* totalCost(Result* result) {
  return result->total_cost;
}

int DEBUG;

void debugMode(char* str) {
  if (strcmp(str, "TRUE")) {
    DEBUG = 0;
  }
  else {
    DEBUG = 1;
  }
}

double maximum(double* arr, int length) {
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

// global state variables: time and cost
double TIME;
// avg for a house (size = 1) is 120 days
// avg for a building is 420 days
// avg for a mall is 1095 days
double COST;
// avg for houses (size = 1) is 100k USD
// avg for a building (size = 5) is 1,500k USD
// avg for malls (size = 10) is 25,000k USD

// actual "size" quantity of the project
int SIZE;



void reset() {
  TIME = 0;
  COST = 0;
  SIZE = 0;
}

void addCost(double c) {
  COST = COST + c;
}

void addTime(double t) {
  TIME = TIME + t;
}

double getCost() {
  return COST;
}

double getTime() {
  return TIME;
}

void setSize(int size) {
  SIZE = size;
}

int getSize() {
  return SIZE;
}


// planning stage
// param is the size of project/number of planners
void planning (int size, int workers) {
  if (workers == 0) {
    printf("There must be at least one planner\n");
    exit(1);
  }

  if (size < 1 || size > 10) {
    printf("Size must be between 1-10\n");
    exit(1);
  }

  setSize(size);

  // this models a simple diminishing returns
  double expected_time = size*5*exp(-0.1*workers) + (2*size);
  // time uncertainity increases as project is bigger
  double time_spread = 0.1 * (workers + size);
  // use normal dist. for time
  double time = abs(normal_dist(expected_time, time_spread));

  // cost depends linearly on size AND time (for maintenance)
  double expected_cost = 0.1 * (workers + size + time);
  // cost uncertainty depends on the amount of planners
  double cost_unc = 0.1 * workers;
  // use normal dist for now
  double cost = abs(normal_dist(expected_cost, cost_unc));

  addTime(time);
  addCost(cost);

  if (DEBUG == 0) {
    printf("Planning time: %.1f\n", time);
    printf("Planning cost: %.1f\n\n", cost);
  }
}

// param is the amount of materials needed
double execution(int labor) {
  if (labor == 0) {
    printf("At least 1 labor. You can't build a house out of thin air.\n");
    exit(1);
  }

  // cost
  double laborCost = labor * abs(normal_dist(10, 0.2));

  // sum all the material costs
  /* double exteriors = amounts[0] * normal_dist(47, 16.63); */
  /* double interiors = amounts[1] * normal_dist(25.94, 14.4); */
  /* double roof = amounts[2] * normal_dist(33.99, 12.41); */
  int size = getSize();
  double materialCost = size * abs(normal_dist(10, 2));
  double cost = laborCost + materialCost;

  // time depends on the amount of workers and materials, i.e.
  // bigger projects need more time
  // int amount = amounts[0] + amounts[1] + amounts[2];
  //double expected_time = pow(((0.5/SIZE) * labor), 3) - (0.6 * labor) + (SIZE*10);
  double expected_time = size*30*exp(-0.5*labor/size) + (100*size);
  double time_unc = 0.2 * labor;
  double time = normal_dist(expected_time, time_unc);

  addCost(cost);
  addTime(time);

  if (DEBUG == 0) {
    printf("Execution time: %.1f\n", time);
    printf("Execution cost: %.1f\n\n", cost);
  }

  return time;
};

void maintenance(double exe_time) {
  double C = getCost();
  int size = getSize();

  // time is the same as the execution because it occurs simultaneously
  double base_cost = size + 0.1 * exe_time;

  // current cost and time taken also affect the spread
  // the larger the cost and time, the more uncertain
  // the cost and time in this phase
  double cost_unc = 0.05*exe_time + 0.2*C;

  // use poisson dist
  double cost = abs(normal_dist(base_cost, cost_unc));

  addCost(cost);

  if (DEBUG == 0) {
    printf("Maintenance cost: %.1f\n\n", cost);
  }

};

// analyze the completion of project
void finalization() {
  double T = getTime();
  double C = getCost();
  /* printf("T: %.2f\n", T); */
  /* printf("C: %.2f\n", C); */
  // expected analysis time increases exponentially with
  // time and cost taken; if the amount of problems is high
  // it is much harder to analyze what went wrong
  double time_exp = 0.1*exp(0.001*T) + 0.2*exp(0.0002*C);
  // time uncertainty increases linearly with time and cost taken
  double time_unc = 0.01 * (T + C);
  double time = abs(normal_dist(time_exp, time_unc));

  // cost again depends on time
  double cost_exp = 0.02 * time;
  double cost_unc = 0.001 * (T + C);
  double cost = abs(normal_dist(cost_exp, cost_unc));

  addCost(cost);
  addTime(time);

  if (DEBUG == 0) {
    printf("Finalization time: %.1f\n", time);
    printf("Finalization cost: %.1f\n\n", cost);
  }

}

Result* runSim(int trials, int size_estimate, int planners, int labor) {
  if (trials == 0) {
    printf("Trials must be at least 1\n");
    exit(1);
  }


  double* timeArr = malloc(trials * sizeof(double));
  double* costArr = malloc(trials * sizeof(double));

  if (timeArr == NULL || costArr == NULL) {
    printf("Not enough memory to malloc results\n");
    exit(1);
  }

  for (int i = 0; i < trials; i++) {
    reset();

    planning(size_estimate, planners);
    double exe_time = execution(labor);
    maintenance(exe_time);
    finalization();

    double time = getTime();
    double cost = getCost();

    timeArr[i] = time;
    costArr[i] = cost;
  }

  Result* result = mkResult(timeArr, costArr);
  return result;
}



/*************************************************************/
/* Generic functions on a double array */

// print the histogram of a given result array (1D)
void printHist(double* results, int n, int binSize) {
  if (results == NULL || n == 0) {
    printf("No results given\n");
    return;
  }

  if (binSize < 1) {
    printf("Bin width must be at least 1\n");
    return;
  }

  int lower = 0;
  int upper = binSize;
  double range = maximum(results, n);
  int steps = 1 + (range / binSize);

  for (int i = 0; i < steps; i++ ) {
    printf("%d:", upper);

    // sum the amount in this bin
    int sum = 0;
    for (int i = 0; i < n; i++) {
      if (lower < results[i] && results[i] <= upper) {
        sum = sum + 1;
      }
    }
    printf("  %d ", sum);

    for (int j = 0; j < (sum/5); j++) {
      printf("x ");
    }
    printf("\n");

    lower = upper;
    upper = upper + binSize;
  }

}

// print a 1D array of doubles
void printArr(double* arr, int length) {
  if (arr == NULL || length == 0) {
    printf("Array cannot be empty\n");
    return;
  }

  for (int i = 0; i < length; i++) {
    printf("%.2f\n", arr[i]);
  }
}
