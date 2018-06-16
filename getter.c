#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "getter.h"

int getter_httpget( int fd, char *url ) {
	int pid;
	int status;

	pid = fork();
	switch( pid ) {
	case 0:
		dup2( fd, 1 );
		close( fd );
		execl( "/usr/bin/curl", "curl", url, NULL );
	case -1:
		return -1;
	default:
		waitpid( pid, &status, 0 );
		if( WIFEXITED( status ) && WEXITSTATUS( status ) == 0 )
			return 0;
		return -1;
	}
}
