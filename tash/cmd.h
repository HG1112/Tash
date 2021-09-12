#ifndef _CMD_H_
#define _CMD_H_

void prompt(void);

int parse(char* line, command* cmd);

int run(command* cmd);

int redirect_input();

int redirect_output();

#endif
