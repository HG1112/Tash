#include <defs.h>
#include <cmd.h>

void interactive() {
				while (true)
				{
								// Print the prompt in interactive mode
								prompt();

								// parse the command
								command cmd*;
								if (parse(getline(), cmd) == 0) {
												// run the command
												run(cmd);
								}

				}
}


void batchmode(int argc, int[] argv) {
				if (argc == 1) { 
								// for each line in file 
								// parse the line 
								// run the commands
				} else {
								error("");
								exit(1);
				}
}
