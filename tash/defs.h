#ifndef _DEFS_H_
#define _DEFS_H_

typedef struct command {
				string path;
				string* args;
				struct command* next;
} command;

#endif
