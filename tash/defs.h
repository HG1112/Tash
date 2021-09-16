#ifndef _DEFS_H_
#define _DEFS_H_

typedef struct Command {
  int len;
  struct Command* next;
  char** args;
} Command;

typedef struct Path {
  int len;
  char** paths;
} Path;

#endif
