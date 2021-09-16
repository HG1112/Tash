#ifndef _CMD_H_
#define _CMD_H_

Command* parse(char* line, Path PATH);

int run(Command* cmd, Path PATH);

#endif
