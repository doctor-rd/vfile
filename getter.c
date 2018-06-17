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
		if( fd != 1 ) {
			if( dup2( fd, 1 ) == -1 )
				_exit( 1 );
			close( fd );
		}
		execl( "/usr/bin/curl", "curl", url, (char*) NULL );
		_exit( 1 );
	case -1:
		return -1;
	default:
		if( waitpid( pid, &status, 0 ) == -1 )
			return -1;
		if( WIFEXITED( status ) && WEXITSTATUS( status ) == 0 )
			return 0;
		return -1;
	}
}
