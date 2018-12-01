#include <stdio.h>
#include <stdlib.h>
#include "Library.h"
#include "rand_dist.h"
#include "print_funcs.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Wrong number of arguments\n");
		return 1;
	}

	char* inputfile = argv[1];
	read_file(inputfile);
	return 0;
}
