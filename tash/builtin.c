#include <stdlib.h>
#include <unistd.h>

void texit() {
				exit(0);
}

void tpath(const char** paths) {
				return;
}

void tcd(const char* path) {
				chdir(path);
}
