#include <unistd.h>
#include <sys/types.h>
#include <attr/xattr.h>
#include <sys/file.h>

#include "getter.h"
#include "fetch.h"

int is_pending( int fd ) {
	char buf;
	int result;

	result = fgetxattr( fd, "user.pending", &buf, 1 );
	return result == 1 && buf == '1';
}

static int dofetch( int fd ) {
	int url_len;
	char url[1024];

	url_len = fgetxattr( fd, "user.xdg.origin.url", url, 1023 );
	if( url_len == -1 )
		return -1;
	url[url_len] = 0;
	if( lseek( fd, 0, SEEK_SET ) != 0 )
		return -1;
	if( getter_httpget( fd, url ) == -1 )
		return -1;
	return fremovexattr( fd, "user.pending" );
}

int fetch_pending( int fd ) {
	int result = -1;

	if( flock( fd, LOCK_EX ) == -1 )
		return -1;
	if( is_pending( fd ) )
		result = dofetch( fd );
	flock( fd, LOCK_UN );
	return result;
}
