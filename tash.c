#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

// constants
const char* delim = " \n";
const char* amp = "&";

// length variables
int plen = 2;
int clen = 0;

// paths
char** paths;

// builtin functions
int tcd(char** cmd);
int texit();
int tpath(char** cmd);

// parse and run the commands
int parse_run(char* cmd);

// modes
void interactive();
void batch(char* file);

//utils
char* trim(char* line);
int redirect_output(char* file);
void error();
void prompt();
char* executable(char* line);


int texit() {
  exit(0);
  return 0;
}

int tpath(char** cmd) {
  if (clen < 2) return -1;
  free(paths);
  plen = clen + 1;
  paths = malloc(sizeof(char*) * plen);
  paths[0] = ".";
  paths[1] = "";
  int i;
  for (i = 1; i < clen; i++) paths[i+1] = cmd[i];
  return 0;
}

int tcd(char** cmd) {
  if (clen != 2) return -1;
  return chdir(cmd[1]);
}

int parse_run(char* line) {

  line = trim(line);
  if (*line == 0) return 0;

  char* token = NULL;
  char*	next = NULL;
  char** cmd = NULL;

  token = strtok(strdup(line), delim);
  if (token == NULL) return -1;

  char* ex = executable(token);
  if (ex == NULL) return -1;

  clen = 0;
  while(token != NULL) {
    clen++;
    token = strtok(NULL, delim);
    if (token != NULL && strcmp(token , amp) == 0) break;
    next = token;
  }

  cmd = malloc(clen * sizeof(char*));
  cmd[0] = ex;

  token = strtok(strdup(line), delim);
  int i;
  for (i=1; i < clen; i++) {
    token = strtok(NULL, delim);
    cmd[i] = token;
  }

  int child_pid = -1, ret = 0;
  if (strcmp(ex, "exit") == 0) {
    ret = texit();
  } else if (strcmp(ex, "cd") == 0) {
    ret = tcd(cmd);
  } else if (strcmp(ex, "path") == 0) {
    ret = tpath(cmd);
  }  else {
    child_pid = fork();
    if (child_pid == 0) ret = execv(ex, cmd); 
  }
  if (ret != 0) error();


  if (next != NULL) return parse_run(next);

  int status; 
  if (child_pid != -1 && waitpid(child_pid, &status, WUNTRACED | WCONTINUED) == -1) return -1;

  return 0;
}

void interactive() {
  while (1)
  {
    char* line = NULL;
    size_t len = 0;

    prompt();

    if (getline(&line, &len, stdin) != -1) {
        if (parse_run(line) == -1) error();
    }
  }
}


void batch(char* file) {
  if (file != NULL) { 
    FILE* fp = NULL;
    char* line = NULL;
    size_t len = 0;

    fp = fopen(file, "r");
    if (fp == NULL) {
      error();
      exit(1);
    }

    if (getline(&line, &len, fp) != -1) {
        if (parse_run(line) == -1) error();
    }
  } else {
    error();
    exit(1);
  }
}

void prompt() {
  char prompt_message[7] = "tash> ";
  write(STDOUT_FILENO, prompt_message, strlen(prompt_message));
}

void error() {
  char error_message[30] = "An error has occurred\n"; 
  write(STDERR_FILENO, error_message, strlen(error_message));
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
  close(STDOUT_FILENO);
  return open(file, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
}

char* executable(char* name) {
  if (strcmp(name, "exit") == 0 || strcmp(name, "cd") == 0 || strcmp(name, "path") == 0) {
    return name;
  } else {
    int ac = -1;
    char* ex = NULL;
    int i;
    for (i = 0; i < plen; i++)  {
      ex = malloc(strlen(paths[i]) + 1 + strlen(name));
      strcpy(ex, paths[i]);
      strcat(ex, "/");
      strcat(ex, name);
      ac = access(ex, F_OK);
      if (ac == 0) return ex;;
      free(ex);
    } 
    return NULL;
  }
}

// Main
int main(int argc, char** argv) {

  paths = malloc(sizeof(char*) * plen);
  paths[0] = ".";
  paths[1] = "";

  if (argc > 2) {
    error();
    exit(1);
  }
  if (argc == 2) {
    batch(argv[1]);
  } else {
    interactive();
  }
  return 0;
}

