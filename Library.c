#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
double COST;

// actual "size" quantity of the project
double SIZE;

double normal(double mean, double spread) {
  return mean;
}

double poisson(double mean, double spread) {
  return mean;
}

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
  if (workers == 0 || size == 0) {
    printf("Planners must be at least 1\n");
    return;
  }

  setSize(size);

  // at first, more workers mean less time, but too many cooks...
  // double expected_time = pow((0.1 * workers), 3) - (0.4 * workers) + (5*size);
  double expected_time = size*10*exp(-0.1*workers) + (5*size);
  // time uncertainity increases as project is bigger
  double time_spread = 0.1 * (workers + size);
  // use normal dist. for time
  double time = normal(expected_time, time_spread);

  // cost depends linearly on size AND time (for maintenance)
  double expected_cost = 3 * (workers + size + time);
  // cost uncertainty depends on the amount of planners
  double cost_unc = 0.1 * workers;
  // use normal dist for now
  double cost = normal(expected_cost, cost_unc);

  // increment the total time
  addTime(time);
  addCost(cost);

  printf("Planning time: %.1f\n", time);
  printf("Planning cost: %.1f\n\n", cost);
}

// param is the amount of materials needed
double execution(int labor, int* amounts) {
  if (labor == 0 || amounts == NULL) {
    printf("Project cannot be empty\n");
    return 0;
  }

  // cost
  double laborCost = labor * normal(10, 0.2);

  // sum all the material costs
  double exteriors = amounts[0] * normal(47, 16.63);
  double interiors = amounts[1] * normal(25.94, 14.4);
  double roof = amounts[2] * normal(33.99, 12.41);
  double cost = laborCost + exteriors + interiors + roof;

  // time depends on the amount of workers and materials, i.e.
  // bigger projects need more time
  int amount = amounts[0] + amounts[1] + amounts[2];
  //double expected_time = pow(((0.5/SIZE) * labor), 3) - (0.6 * labor) + (SIZE*10);
  int size = getSize();
  double expected_time = size*50*exp(-0.05*labor) + (15*size);
  double time_unc = 0.2 * labor + 0.05 * amount;
  double time = normal(expected_time, time_unc);

  addCost(cost);
  addTime(time);

  printf("Execution time: %.1f\n", time);
  printf("Execution cost: %.1f\n\n", cost);

  return time;
};

void maintenance(double exe_time) {
  double C = getCost();
  int size = getSize();

  // time is the same as the execution because it occurs simultaneously
  double base_cost = size + 0.01 * exe_time;

  // current cost and time taken also affect the spread
  // the larger the cost and time, the more uncertain
  // the cost and time in this phase
  double cost_unc = 1.5*exe_time + 0.7*C;

  // use poisson dist
  double cost = poisson(base_cost, cost_unc);

  addCost(cost);
  printf("Maintenance cost: %.1f\n\n", cost);

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
  double time_exp = 0.1*exp(0.0001*T) + 0.2*exp(0.0002*C) + 5.0;
  // time uncertainty increases linearly with time and cost taken
  double time_unc = 0.5 * (T + C);
  double time = poisson(time_exp, time_unc);

  // cost again depends on time
  double cost_exp = 60 * time;
  double cost_unc = 0.7 * (T + C);
  double cost = poisson(cost_exp, cost_unc);

  addCost(cost);
  addTime(time);

  printf("Finalization time: %.1f\n", time);
  printf("Finalization cost: %.1f\n\n", cost);

}

double** runSim(int trials, int size_estimate, int planners, int labor, int* materials) {
  double** results = malloc( 2 * sizeof(double*) );
  results[0] = malloc(trials * sizeof(double));
  results[1] = malloc(trials * sizeof(double));

  if (trials == 0) {
    printf("Trials must be at least 1\n");
    return NULL;
  }

  if (results == NULL) {
    printf("Not enough memory to malloc\n");
    return NULL;
  }

  for (int i = 0; i < trials; i++) {
    reset();

    planning(size_estimate, planners);
    double exe_time = execution(labor, materials);
    maintenance(exe_time);
    finalization();

    double time = getTime();
    double cost = getCost();

    results[0][i] = time;
    results[1][i] = cost;
  }

  return results;
}

// print the histogram to a file
void printHist(double* results, int n, int binSize) {
  if (results == NULL ) {
    printf("No results given\n");
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
    printf("  %d\n", sum);

    lower = upper;
    upper = upper + binSize;
  }

}

void printArr(double* arr, int length) {
  for (int i = 0; i < length; i++) {
    printf("%.2f\n", arr[i]);
  }
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
    printf("Wrong number of arguments\n");
    return 1;
  }

  int size_estimate = atoi(argv[1]);
  int planners = atoi(argv[2]);
  int labors = atoi(argv[3]);
  int materials[3] = {5, 5, 6};
  int trials = atoi(argv[4]);

  double** results = runSim(trials, size_estimate, planners, labors, materials);
  printf("Total time\n");
  printArr(results[0], trials);
  printf("Total cost\n");
  printArr(results[1], trials);
  printf("\n");

  /* printf("Time distribution:\n"); */
  /* printHist(results[0], trials, 200); */
  /* printf("\n"); */
  /* printf("Cost distribution:\n"); */
  /* printHist(results[1], trials, 200); */

  return 0;
}