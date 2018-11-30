#include <stdio.h>
#include <stdlib.h>
#include "Library.h"


int main(int argc, char* argv[]) {
  if (argc != 6) {
    printf("Wrong number of arguments\n");
    return 1;
  }

  debugMode(argv[5]);

  int size_estimate = atoi(argv[1]);
  int planners = atoi(argv[2]);
  int labors = atoi(argv[3]);
  // int materials[3] = {5, 5, 6};
  int trials = atoi(argv[4]);

  Result* results = runSim(trials, size_estimate, planners, labors);
  if (results == NULL) {
    printf("No results were generated\n");
    return 1;
  }
  double* time_taken = totalTime(results);
  double* cost_involved = totalCost(results);
  /* printf("Total time\n"); */
  /* printArr(results[0], trials); */
  /* printf("Total cost\n"); */
  /* printArr(results[1], trials); */
  /* printf("\n"); */

  printf("Time distribution:\n");
  printHist(time_taken, trials, 10);
  printf("Expected value  Standard Dev\n");
  double expected_time = expectation(time_taken, trials);
  double dev_time = deviation(time_taken, trials);
  printf("%.2f         %.2f\n", expected_time, dev_time);

  printf("\n");
  printf("Cost distribution:\n");
  printHist(cost_involved, trials, 200);
  printf("Expected value  Standard Dev\n");
  double expected_cost = expectation(cost_involved, trials);
  double dev_cost = deviation(cost_involved, trials);
  printf("%.2f         %.2f\n", expected_cost, dev_cost);


  return 0;
}
