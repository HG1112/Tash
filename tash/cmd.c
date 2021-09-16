#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "defs.h"
#include "utils.h"

Command* parse(char* line, Path PATH) {
  const char* delim = " \n";
  const char* amp = "&";
  char* token = NULL;
  char*	next = NULL;
  int len = 0;

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
  struct Command* cmd = malloc( sizeof(*cmd) + (len+1) * sizeof(char*) );
  executable(token, PATH);
  if (token == NULL) return NULL;

  cmd->args = malloc((len + 1) * sizeof(char*));
  cmd->args[0] = token;
  for (int i = 1; i <= len; i++) cmd->args[i] = strtok(NULL, delim);
  cmd->len = len;
  if (next != NULL) cmd->next = parse(next, PATH);

  return cmd;
}

int run(Command* cmd, Path PATH) {
  int parallel = 0;
  while(cmd != NULL) {
    if (fork() == 0) {
      if (execv(cmd->args[0], cmd->args) == -1) error();
      cmd = cmd->next;
    } else {
      cmd = cmd->next;
      run(cmd, PATH);
      parallel++;
    }
  }
  while(parallel) {
    wait(NULL);
    parallel--;
  }
  return 0;
}
