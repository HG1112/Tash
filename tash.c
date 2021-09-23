/**
 * Author   : Harish Gontu, Sharath Nakka
 * Purpose  : Implementation of TexAs SHell 
 * Language : C
 * 
 */

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

/**
 * Constants 
 */
const char* whitespace = " \t\n";
const char amp = '&';
const char gt = '>';

/**  -------- Global variables ----------  */

/* Length variables for path and command */
int plen = 1;
int clen = 0;

/* Placeholder for paths */
char** paths;

/**  -------- STRING utilities ----------  */

/**
 * @brief print the predefined error "An error has occured"
 * @param
 * @return
 */
void error()
{
  char error_message[30] = "An error has occurred\n"; 
  write(STDERR_FILENO, error_message, strlen(error_message));
}

/**
 * @brief print the predefined prompt "tash >"
 * @param
 * @return
 */
void prompt()
{
  char prompt_message[7] = "tash> ";
  write(STDOUT_FILENO, prompt_message, strlen(prompt_message));
}

/**
 * @brief return number of occurences of char c in a line
 * @param line to count occurences and char to count occurences of
 * @return number of occurences
 */
int num_chars(char* line, const char c)
{
  if (line == NULL) return 0;
  int i, num = 0;
  for (i = 0; line[i]; i++) {
    if (line[i] == c) num++;
  }
  return num;
}

/**
 * @brief return trimmed string
 * @param string to be trimmed
 * @return trimmed string
 */
char* trim(char* line)
{
  /* https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way */
  while(isspace((unsigned char) *line)) line++;
  if (*line == 0) return line;
  char* last = line + strlen(line) - 1;
  while (last >= line && isspace((unsigned char) *last)) last--;
  last[1] = '\0';
  return line;
}



/**
 * @brief return number of tokens after line is split by a separator
 * @param line to be split and separator to be split by
 * @return number of splitted tokens
 */
int num_tokens(char* line, const char sep)
{
  /* http://c-faq.com/lib/strtok.html */
  int num = 0;
  char* p = strdup(line);
  while(1) {
    char *p2 = strchr(p, sep);
    num++;
    if(p2 != NULL) *p2 = '\0';
    if(p2 == NULL) break;
    p = p2 + 1;
  }
  return num;
}

/**
 * @brief return array of tokens after line is split by a separator
 * @param line to be split and separator to be split by
 * @return array of splitted tokens
 */
char** split(char* line, const char sep)
{

  int num = num_tokens(line, sep);
  char** result = malloc(num * sizeof(char*));
  char* p = strdup(line);
  int i = 0;
  for ( ; i < num && p != NULL; i++) {
    char *p2 = strchr(p, sep);
    if (p2 != NULL)
    {
      *p2 = '\0';
      result[i] = strdup(p);
      p = p2 + 1;
    }
    else  {
      result[i] = strdup(p);
      p = NULL;
    }
  }
  return result;
}

/**
 * @brief return number of tokens after line is split by a separator
 * @param line to be split and separator to be split by
 * @return number of splitted tokens
 */
int num_tokens_str(char* line, const char* sep)
{
  int num = 0;
  char* tk;
  for (tk = strtok(strdup(line), sep); tk != NULL; tk = strtok(NULL, sep)) num++;
  return num;
}

/**
 * @brief return array of tokens after line is split by a separator
 * @param line to be split and separator to be split by
 * @return array of splitted tokens
 */
char** split_str(char* line, const char* sep)
{
    int num = num_tokens_str(line, sep);
    char** result = malloc(num * sizeof(char*));
    char* tk;
    int i = 0;
    for (tk = strtok(strdup(line), sep); i < num && tk != NULL; tk = strtok(NULL, sep)) result[i++] = tk;
    return result;
}

/**
 * @brief copy elements from 'from' to 'to' array as per given size
 * @param from and to array of strings, size to be copied
 * @return 
 */
void copy(char** from , char** to, int size) {
  int i = 0;
  for (; i < size; i++) to[i] = from[i];
}

/**  -------- FILE utilities ----------  */

/**
 * @brief redirect stdout and stderr to file
 * @param path to file 
 * @return 0 if redirect success else -1
 */
int output(char* file)
{
  fclose(stdout);
  fclose(stderr);
  if (freopen(file, "w+", stdout) == NULL) return -1;
  if (freopen(file, "w+", stderr) == NULL) return -1;
  return 0;
}

/**
 * @brief finds executable from all stored paths based on provided name(rel.path also allowed)
 * @param name of executable or relative path
 * @return proper path to the executable
 */
char* executable(char* name)
{
  // check for builtin functions before finding in paths
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

/**  -------- Builtin functions ----------  */

/**
 * @brief change present working directory to provided path
 * @param path of the new directory
 * @return code 0 for success or -1 for failure
 */
int tcd(char** cmd)
{
  if (clen == 1) {
    // change directory to HOME if no argument provided
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


/**
 * @brief exit from shell
 * @param 
 * @return -1 if it fails
 */
int texit(void)
{
  // fail with error if exit called with more than one arg
  if (clen != 1) return -1;
  exit(0);
  return 0;
}

/**
 * @brief override paths based of list of paths provided as arguments of command
 * @param list of paths as args of command
 * @return 0 for successful override else -1
 */
int tpath(char** cmd)
{
  char** new_paths = malloc(sizeof(char*) * (clen-1));
  int i;
  for (i = 0; i < clen-1; i++) {
    // fail override paths if any arg of path is inaccessible
    if (access(cmd[i+1], F_OK) != 0) return -1;
    new_paths[i] = cmd[i+1];
  }
  free(paths);
  paths = new_paths;
  plen = clen-1;
  return 0;
}


/**  -------- Command ----------  */

/**
 * @brief parse the given command for executable and arguments along with redirection
 * @param command as string
 * @return process id of child process or return code for execution of builtin function
 */
int parse_run(char* command)
{
  char *line = trim(command);
  if (*line == 0) return 0;

  char** cmd = NULL;

  // Redirection and its relevant checks
  char* redirect = NULL;
  if (strchr(line, gt) != NULL) {

    // multiple redirection undefined
    clen = num_tokens(line, gt);
    if (clen != 2) return -1;

    cmd = split(line, gt);
    line = trim(cmd[0]);

    // left operand cannot be empty
    if (*line == 0) return -1;

    // right operand cannot be empty nor should it have spaces in it
    redirect = trim(cmd[1]);
    if (*redirect == 0 || num_tokens_str(redirect, whitespace) != 1) return -1;
  }

  // Construction of command as an array of arguments with file path as args[0]
  clen = num_tokens_str(line, whitespace);
  cmd = malloc((clen + 1) * sizeof(int));
  copy(split_str(line, whitespace), cmd, clen);
  cmd[clen] = NULL;

  // Confirm the existence of executable to run
  char* ex = executable(cmd[0]);
  if (ex == NULL) return -1;
  cmd[0] = ex;

  // Run
  int child_pid = 0, ret = 0;
  if (strcmp(ex, "exit") == 0) {
    // Built-in functions run in parent process
    ret = texit();
  } else if (strcmp(ex, "cd") == 0) {
    // Built-in functions run in parent process
    ret = tcd(cmd);
  } else if (strcmp(ex, "path") == 0) {
    // Built-in functions run in parent process
    ret = tpath(cmd);
  }  else {

    // Non-built-in functions run in child processes
    child_pid = fork();
    if (child_pid == 0) {
      // redirect if asked for
      if (redirect != NULL) output(redirect);
      ret = execv(ex, cmd); 
    } else {
      ret = child_pid;
    }
  }

  // Return child process id if forked or else return code of builtin function
  return ret;
}


/**
 * @brief run command whilst waiting for child process , if any forked
 * @param command as string
 * @return 
 */
void shell(char* command)
{
    int ps = num_tokens(command, amp);
    char** cmds = split(command, amp);
    int* process = malloc(ps * sizeof(int));

    int idx , status;
    for (idx = 0; idx < ps; idx++) {
      cmds[idx] = trim(cmds[idx]);
      if (idx > 0 && *cmds[idx-1] == 0) {
        error();
        return;
      }
    }
    for (idx = 0; idx < ps; idx++) process[idx] = parse_run(strdup(cmds[idx]));
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

/**  -------- Mode ----------  */

/**
 * @brief execute the shell in interactive mode
 * @param
 * @return 
 */
void interactive()
{
  char* command = NULL;
  size_t len = 0;
  while(1)
  {
    prompt();
    if (getline(&command, &len, stdin) > 0) shell(command);
  }
}


/**
 * @brief execute the shell in batch mode
 * @param path to script or file to execute in batch mode
 * @return 
 */
void batch(char* file)
{
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

    // Batch file cannot exit if exit not provided in the script
    error();
    exit(1);
  } else {
    error();
    exit(1);
  }
}


/**  -------- Mode ----------  */

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

