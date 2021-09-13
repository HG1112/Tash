#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "cmd.h"
#include "utils.h"

void interactive() {
				while (1)
				{
								char* line = NULL;
								size_t len = 0;

								prompt();

								if ((getline(&line, &len, stdin) != -1)) {
												struct command* cmd = parse(line);
												if (cmd == NULL) error();
												run(cmd);
								}

				}
}


void batchmode(char* file) {
				if (file != NULL) { 
								FILE* fp = NULL;
								char* line = NULL;
								size_t len = 0;

								fp = fopen(file, "r");
								if (fp == NULL) exit(1);

								if ((getline(&line, &len, fp) != -1)) {
												struct command* cmd = parse(line);
												if (cmd == NULL) error();
												run(cmd);
								}

								exit(0);
				} else {
								error();
								exit(1);
				}
}
