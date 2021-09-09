#ifndef _CMD_H_
#define _CMD_H_


void prompt(void);

int parse(string cmd, string* args);

int run(string cmd);

int redirect_input();

int redirect_output();

#endif // _CMD_H_
