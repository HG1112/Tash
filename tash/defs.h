#ifndef _DEFS_H_
#define _DEFS_H_

typedef struct command {
				struct command* next;
				char** args;
} command;

#endif
