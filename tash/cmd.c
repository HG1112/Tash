#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include "defs.h"
#include "utils.h"

command* parse(char* line) {
				const char* delim = " \n";
				const char* amp = "&";
				char* token = NULL;
				char*	next = NULL;
				size_t len = 0;

				token = strtok(line, delim);
				if (token == NULL) return NULL;
				while(token != NULL) {
								token = strtok(NULL, delim);
								if (token != NULL) {
												if (strcmp(token, amp)) break;
												len++;
												next = token;
								}
				}

				token = strtok(line, delim);
				struct command* cmd = malloc( sizeof(*cmd) + (len+1) * sizeof(char*) );
				cmd->args[0] = token;
				for (int i = 1; i <= len; i++) cmd->args[i] = strtok(NULL, delim);

				if (next != NULL) cmd->next = parse(next);

				return cmd;
}

int run(command* cmd) {
				int parallel = 0;
				while(cmd != NULL) {
								int child_pid = fork();
								if (child_pid == 0) {
												for (int i = 0 ; i < 5; i++) printf("Args %d : %s\n", i, cmd->args[i]);
												int ret  = execv(cmd->args[0], cmd->args);
												if (ret == -1) error();
												cmd = cmd->next;
								} else {
												cmd = cmd->next;
												run(cmd);
												parallel++;
								}
				}
				while(parallel) {
								wait(NULL);
								parallel--;
				}
				return 0;
}
