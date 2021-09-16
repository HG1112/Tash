#ifndef _UTILS_H_
#define _UTILS_H_

void prompt(void);

void error(void);

int redirect_output(char* file);

char* trim(char* line);

void executable(char* name, char** PATH);

#endif
