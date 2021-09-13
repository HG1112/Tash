#include <stdio.h>

void prompt(void) {
				printf("tash> ");
}

void error(char* msg) {
				printf("%s\n", msg);
}

int redirect_output(char* file) {
				freopen(file, "w+", stdout);
}
