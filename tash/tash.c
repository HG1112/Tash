#include <stdio.h>
#include <stdlib.h>
#include "mode.h"


int main(int argc, char** argv) {
  struct Path PATH;
  PATH.paths = malloc(2 * sizeof(char*));
  PATH.paths[0] = ".";
  PATH.paths[1] = "";
  PATH.len = 2;

	if (argc == 2) {
			batchmode(argv[1], PATH);
	} else {
			interactive(PATH);
	}
	return 0;
}
