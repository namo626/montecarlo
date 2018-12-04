#ifndef PRINT_FUNCS_H_
#define PRINT_FUNCS_H_

/*
 * Reads an input file, where each line has the following format:
 * SIZE PLANNERS WORKERS TRIALS DEBUG_MODE OUTPUT_FILE
 * Then uses the simulation library to run a simulation with the specified
 * values. After simulation is finished the result is printed to a
 * file with name specified in OUTPUT_FILE.
 * param configfile the name of the configuration file
 */
void read_file(char* configfile);

/*
 * Prints the output to a file with name outfile.
 * param results the results obtained from the simulation run
 * param trials the number of times simulation was run
 * param outfile the name of the output file
 */
void print_output(Result* results, int trials, char* outfile);

#endif /* PRINT_FUNCS_H_ */
