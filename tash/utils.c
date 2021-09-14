#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void prompt(void) {
  printf("tash> ");
}

void error() {
  printf("An error has occured\n");
}

int redirect_output(char* file) {
  FILE* fp = freopen(file, "w+", stdout);
  if (fp == NULL) return -1;
  return 0;
}

void executable(char* name, char** PATH) {
  int ac = -1;
  int len = sizeof(PATH)/sizeof(char*);
  for (int i = 0; i < len; i++)  {
    char* ex = malloc(100 * sizeof(char));
    strcpy(ex, PATH[i]);
    strcat(ex, "/");
    strcat(ex, name);
    ac = access(ex, F_OK);
    if (ac == 0) {
      name = ex;
      return;
    }
    free(ex);
  }
  name = NULL;
}
