void exit() {
				exit(0)
}

void path(string* paths) {
				int n = len(paths);
				PATH = (string) malloc(n);
				for (int i = 0; i < n; i++) {
								PATH[i] = *(paths+i);
				}
}

void cd(string path) {
				chdir(path);
}
