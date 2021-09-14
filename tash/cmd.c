#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "defs.h"
#include "utils.h"
#include "builtin.h"

command* parse(char* line, char** PATH) {
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
  struct command* cmd = malloc( sizeof(*cmd) + (len+1) * sizeof(char*) );
  executable(token, PATH);
  if (token == NULL) return NULL;

  cmd->args = malloc((len + 1) * sizeof(char*));
  cmd->args[0] = token;
  for (int i = 1; i <= len; i++) cmd->args[i] = strtok(NULL, delim);

  if (next != NULL) cmd->next = parse(next, PATH);

  return cmd;
}

int run(command* cmd, char** PATH) {
  int parallel = 0;
  while(cmd != NULL) {
    int child_pid = fork();
    if (child_pid == 0) {
      if (strcmp(cmd->args[0], "exit"))
        texit();
      else if (strcmp(cmd->args[0], "cd"))
        tcd(cmd);
      else if (strcmp(cmd->args[0], "path"))
        tpath(PATH, cmd);
      else if (execv(cmd->args[0], cmd->args) == -1)
        error();
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
