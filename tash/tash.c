#include <stdio.h>
#include <stdlib.h>
#include "mode.h"


int main(int argc, char** argv) {
  char** PATH = malloc(2 * sizeof(char*));
  PATH[0] = ".";
  PATH[1] = "";

	if (argc == 2) {
			batchmode(argv[1], PATH);
	} else {
			interactive(PATH);
	}
	return 0;
}
