#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <attr/xattr.h>

int main( int argc, char* const argv[] ) {
	int fd;
	char *url;
	char *filename;

	if( argc < 2 ) {
		printf( "Usage: %s URL\n", argv[0] );
		return 0;
	}

	url = argv[1];
	filename = strrchr( url, '/' );
	if( filename == NULL ) {
		fprintf( stderr, "Unable to determine filename\n" );
		return 1;
	}
	filename++;

	fd = open( filename, O_EXCL|O_CREAT, 0666 );
	if( fd == -1 ) {
		perror( filename );
		return 1;
	}
	if( flock( fd, LOCK_EX ) == -1 ) {
		perror( filename );
		unlink( filename );
		return 1;
	}
	if( fsetxattr( fd, "user.pending", "1", 1, 0 ) == -1 || fsetxattr( fd, "user.xdg.origin.url", url, strlen( url ), 0 ) == -1 ) {
		perror( filename );
		unlink( filename );
		return 1;
	}

	return 0;
}
