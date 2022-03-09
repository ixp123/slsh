#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

int hostname() {

	int gethostname(char *name, size_t namelen); 

	char hostname[HOST_NAME_MAX + 1];
	gethostname(hostname, HOST_NAME_MAX + 1);

	printf("hostname: %s\n", hostname);
}
