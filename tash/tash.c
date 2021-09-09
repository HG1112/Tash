#include <mode.h>

int main(int argc, int argv[]) {
	if (argc == 0) {
			interactive();
	} else {
			batchmode(argc, argv);
	}
	return 0;
}
