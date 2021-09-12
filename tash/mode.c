#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "cmd.h"

void interactive() {
				while (1)
				{
								struct command* cmd = malloc( sizeof(struct command) );
								char* line = NULL;
								size_t len = 0;

								prompt();

								if ((getline(&line, &len, stdin) != -1) && (parse(line, cmd) == 0)) {
												run(cmd);
								}

				}
}


void batchmode(char* file) {
				if (1) { 
								FILE* fp;
								struct command* cmd = malloc( sizeof(struct command) );
								char* line = NULL;
								size_t len = 0;

								fp = fopen(file, "r");
								if (fp == NULL) exit(1);
								if ((getline(&line, &len, fp) != -1) && (parse(line, cmd) == 0)) {
												run(cmd);
								}
								exit(0);
				} else {
								/* error(""); */
								exit(1);
				}
}
