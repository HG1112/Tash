#ifndef _DEFS_H_
#define _DEFS_H_

typedef char* string;

typedef struct command {
				string path;
				string* args;
				struct command* next;
} command;
#endif
