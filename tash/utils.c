#include <stdio.h>

void prompt(void) {
				printf("tash> ");
}

void error() {
				printf("An error has occured\n");
}

int redirect_output(char* file) {
				FILE* fp = freopen(file, "w+", stdout);
				if (fp == NULL) return -1;
				return 0;
}
