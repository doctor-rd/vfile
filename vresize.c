#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>

#include "fetch.h"

int main( int argc, char* const argv[] ) {
	int fd;
	long newsize;

	if( argc < 3 ) {
		printf( "Usage: %s FILENAME NEWSIZE\n", argv[0] );
		return 0;
	}

	newsize = atol( argv[2] );
	fd = open( argv[1], O_RDWR );
	if( fd == -1 ) {
		perror( argv[1] );
		return 1;
	}
	if( flock( fd, LOCK_EX ) == -1 ) {
		perror( argv[1] );
		return 1;
	}
	if( !is_pending( fd ) ) {
		printf( "%s is not a vfile.\n", argv[1] );
		return 0;
	}
	if( ftruncate( fd, newsize ) == -1 ) {
		perror( argv[1] );
		return 1;
	}

	return 0;
}
