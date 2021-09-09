
void prompt() {
		printf("tash> ");
}


int parse(string line, command* cmd) {

		return ret;
}

int run(command* cmd) {
		while(cmd != NULL) {
				int child_pid = fork();
				if (child_pid == 0) {
						int ret = execv(cmd->path, cmd->args);
						error("Unable to run command");
				} else {
						waitpid(child_pid);
						run(cmd->next);
				}
		}
}

int redirect_input() {

}


int redirect_output() {

}
