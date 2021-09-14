#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "defs.h"
#include "utils.h"

void texit() {
				exit(0);
}

void tpath(char** PATH, command* cmd) {
				free(PATH);
        int len = sizeof(cmd->args) / sizeof(char*);
				if (len < 1) error();
				PATH = malloc(sizeof(char*) * (len + 1));
        PATH[0] = ".";
        PATH[1] = "";
				for (int i = 2; i < len + 1; i++) PATH[i] = cmd->args[i];
}

void tcd(command* cmd) {
        int len = sizeof(cmd->args)/sizeof(char*);
        if (len < 2) 
          error();
        else {
          int ret = chdir(cmd->args[1]);
          if (ret == -1) error();
        }
}
