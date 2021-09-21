#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

// constants
const char* whitespace = " \t\n";
const char* amp = "&";
const char* gt = ">";

// length variables
int plen = 1;
int clen = 0;

// paths
char** paths;
char* executable(char* line);

// builtin functions
int tcd(char** cmd);
int texit();
int tpath(char** cmd);

// parse and run the commands
int parse_run(char* cmd);

// modes
void interactive();
void batch(char* file);

// shell
void shell(char* command);
int output(char* file);
void error();
void prompt();

//utils
int num_chars(char* line, const char c);
int num_tokens(char* line, const char* sep);
char* trim(char* line);
char** split(char* line, const char* sep);


int texit() {
  if (clen != 1) return -1;
  exit(0);
  return 0;
}

int tpath(char** cmd) {
  char** new_paths = malloc(sizeof(char*) * (clen-1));
  int i;
  for (i = 0; i < clen-1; i++) {
    if (access(cmd[i+1], F_OK) != 0) return -1;
    new_paths[i] = cmd[i+1];
  }
  free(paths);
  paths = new_paths;
  plen = clen-1;
  return 0;
}

int tcd(char** cmd) {
  if (clen == 1) {
    const char* home;
    if ((home = getenv("HOME")) == NULL) {
      home = getpwuid(getuid())->pw_dir;
      return chdir(home);
    }
    return -1;
  }
  if (clen != 2) return -1;
  return chdir(cmd[1]);
}

int parse_run(char* command) {

  char *line = trim(command);
  if (*line == 0) return 0;

  char** cmd = NULL;
  char* redirect = NULL;
  if (strchr(line, '>') != NULL) {
    clen = num_tokens(line, gt);
    if (clen != 2) return -1;
    cmd = split(line, gt);
    redirect = cmd[1];
    if (num_chars(redirect, ' ') != 0) return -1;
    line = cmd[0];
  }

  clen = num_tokens(line, whitespace);
  char* tk;
  cmd = malloc((clen + 1) * sizeof(char*));
  int i = 0;
  for (tk = strtok(strdup(line), whitespace); i < clen && tk != NULL; tk = strtok(NULL, whitespace)) cmd[i++] = tk;
  cmd[clen] = NULL;
  char* ex = executable(cmd[0]);
  if (ex == NULL) return -1;
  cmd[0] = ex;

  int child_pid = 0, ret = 0;
  if (strcmp(ex, "exit") == 0) {
    ret = texit();
  } else if (strcmp(ex, "cd") == 0) {
    ret = tcd(cmd);
  } else if (strcmp(ex, "path") == 0) {
    ret = tpath(cmd);
  }  else {
    child_pid = fork();
    if (child_pid == 0) {
      if (redirect != NULL) output(redirect);
      ret = execv(ex, cmd); 
    } else {
      ret = child_pid;
    }
  }

  return ret;
}

void shell(char* command) {
    // process variables
    int ps = num_tokens(command, amp);
    char** cmds = split(command, amp);
    int* process = malloc(ps * sizeof(int));

    int idx , status;
    for (idx = ps-1; idx >= 0; idx--) process[idx] = parse_run(strdup(cmds[idx]));
    for (idx = 0; idx < ps; idx++) {
      if (process[idx] <= -1)
        error();
      else if (process[idx] != 0) {
        if (waitpid(process[idx], &status, WUNTRACED | WCONTINUED) == -1) error();
      }
    }
    free(process);
    free(cmds);
}

void interactive() {
  char* command = NULL;
  size_t len = 0;
  while(1)
  {
    prompt();
    if (getline(&command, &len, stdin) > 0) shell(command);
  }
}


void batch(char* file) {
  char* command = NULL;
  size_t len = 0;
  if (file != NULL) { 
    FILE* fp = fopen(file, "r");
    if (fp == NULL) 
    {
      error();
      exit(1);
    }
    while (getline(&command, &len, fp) > 0) shell(command);
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

int num_chars(char* line, const char c) {
  if (line == NULL) return 0;
  int i, num = 0;
  for (i = 0; line[i]; i++) {
    if (line[i] == c) num++;
  }
  return num;
}

int num_tokens(char* line, const char* sep) {
  int num = 0;
  char* tk;
  for (tk = strtok(strdup(line), sep); tk != NULL; tk = strtok(NULL, sep)) num++;
  return num;
}

char** split(char* line, const char* sep) {
  int num = num_tokens(line, sep);
  char* tk;
  char** result = malloc(num * sizeof(char*));
  int i = 0;
  for (tk = strtok(strdup(line), sep); i < num && tk != NULL; tk = strtok(NULL, sep)) result[i++] = tk;
  return result;
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

int output(char* file) {
  close(STDOUT_FILENO);
  return open(file, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
}

char* executable(char* name) {
  if (strcmp(name, "exit") == 0 || strcmp(name, "cd") == 0 || strcmp(name, "path") == 0) {
    return trim(name);
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
      if (ac == 0) return trim(ex);
      free(ex);
    } 
    return NULL;
  }
}

// Main
int main(int argc, char** argv) {

  paths = malloc(sizeof(char*) * plen);
  paths[0] = "/bin";

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

