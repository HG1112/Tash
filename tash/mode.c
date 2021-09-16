#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "defs.h"
#include "cmd.h"
#include "utils.h"
#include "builtin.h"

void interactive(char** PATH) {
  while (1)
  {
    char* line = NULL;
    size_t len = 0;

    prompt();

    if (getline(&line, &len, stdin) != -1) {
      line = trim(line);
      if (*line != 0) {
        struct command* cmd = parse(line, PATH);
        if (cmd == NULL) error();

        builtin(PATH, cmd);

        run(cmd, PATH);
      }
    }

  }
}


void batchmode(char* file, char** PATH) {
  if (file != NULL) { 
    FILE* fp = NULL;
    char* line = NULL;
    size_t len = 0;

    fp = fopen(file, "r");
    if (fp == NULL) exit(1);

    if (getline(&line, &len, stdin) != -1) {
      line = trim(line);
      if (*line != 0) {
        struct command* cmd = parse(line, PATH);
        if (cmd == NULL) error();

        builtin(PATH, cmd);

        run(cmd, PATH);
      }
    }
  } else {
    error();
    exit(1);
  }
}
