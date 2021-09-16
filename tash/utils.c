#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void prompt(void) {
  printf("tash> ");
}

void error() {
  printf("An error has occured\n");
}

/* https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way */
char* trim(char* line) {
  while(isspace((unsigned char) *line)) line++;
  if (*line == 0) return line;
  char* last = line + strlen(line) - 1;
  while (last >= line && isspace((unsigned char) *last)) last--;
  last[1] = '\0';
  return line;

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
