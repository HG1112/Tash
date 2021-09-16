#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "utils.h"

void texit() {
  exit(0);
}

void tpath(Path PATH, Command* cmd) {
  free(PATH.paths);
  int len = cmd->len;
  PATH.paths = malloc(sizeof(char*) * (len + 2));
  PATH.paths[0] = ".";
  PATH.paths[1] = "";
  for (int i = 1; i < len + 1; i++) PATH.paths[i+1] = cmd->args[i];
  PATH.len = len+2;
}

void tcd(Command* cmd) {
  int len = cmd->len;
  if (len != 1) 
    error();
  else {
    if (chdir(cmd->args[1]) == -1) error();
  }
}

void builtin(Path PATH, Command* cmd) {

  /*Check for built in first*/
  if (strcmp(cmd->args[0], "exit") == 0)
    texit();
  else if (strcmp(cmd->args[0], "cd") == 0)
    tcd(cmd);
  else if (strcmp(cmd->args[0], "path") == 0)
    tpath(PATH, cmd);

}
