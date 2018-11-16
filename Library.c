#include <stdio.h>
#include <stdlib.h>

// global state variables: time and cost
double TIME;
double COST;

void addCost(double c) {
  COST = COST + c;
}

void addTime(double t) {
  TIME = TIME + t;
}

// planning stage
void planning (double param) {
  double time = randomize();
  double cost = randomize();
  // increment the total time
  addTime(time);
  addCost(time);
}

void execution(double param) {
  double labor = randomize();
  double ceiling = randomize();

  if (constraint(labor, ceiling) == TRUE) {
    // exit program
  }
  else {
    addCost(cost);
    addTime(time);
  }
};
