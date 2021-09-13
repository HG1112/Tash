#ifndef _CMD_H_
#define _CMD_H_

int parse(char* line, command* cmd);

int run(command* cmd);

#endif
