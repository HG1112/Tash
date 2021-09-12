#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include "defs.h"

int prompt() {
				printf("tash> ");
				return 0;
}


int parse(char* line, command* cmd) {
				const char* delim = " ";
				const char* amp = "&";
				char* token = NULL;

				token = strtok(line, delim);
				if (token == NULL) return -1;
				cmd->path = token;

				while (1) {
								token = strtok(NULL, delim);
								if (token == NULL) break;
								if (strcmp(token, amp)) {
												cmd->next = malloc(sizeof(struct command));
												return parse(token, cmd->next);
								}
				}
				return 0;
}

int run(command* cmd) {
				int parallel = 0;
				while(cmd != NULL) {
								int child_pid = fork();
								if (child_pid == 0) {
												execv(cmd->path, cmd->args);
												/* error("Unable to run command"); */
								} else {
												run(cmd->next);
												parallel++;
								}
				}
				while(parallel) {
								wait(NULL);
								parallel--;
				}
				return 0;
}

int redirect_input() {
				return 0;
}


int redirect_output() {
				return 0;
}
