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
  int materials[3] = {5, 5, 6};
  int trials = atoi(argv[4]);

  double** results = runSim(trials, size_estimate, planners, labors, materials);
  /* printf("Total time\n"); */
  /* printArr(results[0], trials); */
  /* printf("Total cost\n"); */
  /* printArr(results[1], trials); */
  /* printf("\n"); */

  printf("Time distribution:\n");
  printHist(results[0], trials, 10);
  printf("\n");
  printf("Cost distribution:\n");
  printHist(results[1], trials, 200);

  return 0;
}
