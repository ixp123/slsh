#include <unistd.h>
#include <stdio.h>
#include <limits.h>

char cwd[PATH_MAX];
if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
} else {
	perror("getcwd() error");
}

