#ifndef _CMD_H_
#define _CMD_H_

command* parse(char* line);

int run(command* cmd);

#endif
