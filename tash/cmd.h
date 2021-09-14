#ifndef _CMD_H_
#define _CMD_H_

command* parse(char* line, char** PATH);

int run(command* cmd, char** PATH);

#endif
