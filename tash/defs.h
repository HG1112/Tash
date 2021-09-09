#ifndef _DEFS_H_
#define _DEFS_H_

typedef char* string;

struct command {
				string path;
				string* args;
				command* next;
}
#endif // _DEFS_H_
