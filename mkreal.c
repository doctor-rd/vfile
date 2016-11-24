#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fetch.h"

int main( int argc, char* const argv[] ) {
	int fd;

	if( argc < 2 ) {
		printf( "Usage: %s FILENAME\n", argv[0] );
		return 0;
	}

	fd = open( argv[1], O_RDWR );
	if( fd == -1 ) {
		perror( argv[1] );
		return 1;
	}
	if( fetch_pending( fd ) == -1 )
		return 1;

	return 0;
}
