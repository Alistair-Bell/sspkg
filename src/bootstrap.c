#include "bootstrap.h"

/* Defines the file privilage bits, root has rwx whilst others have r-- for non root database searches and package info. */
static const uint32_t file_perms = 0x1e4;
/* Defines the desired perms for a directory, this includes the most significent being active (for directory). */
static const uint32_t dir_perms  = 0x41e4;

static int8_t 
handle_dir(const char *path)
{
	if (mkdir(path, file_perms) != 0) {
		register int snapshot = errno;	
		fprintf(stdout, "sspkg-boostrap, unable to create directory %s, error code %d\n", path, snapshot);
		return -1;
	}
	return 0;
}

/* This function is very methodical and verbose, code may be long but does the job well. */
int8_t
bootstrap(void)
{
	/* Require that bootstrap is run as root, this ensures that all permissions can be set correctly and simplifies the process. */
	if (getuid() != 0) {
		fprintf(stderr, "sspkg-boostrap: boostrap must be run as root, full filesystem access is manditory!\n");
		return -1;
	}

	/* 
     * Validate that the databse directory exists, set the permissions correctly and remove any imposter files.
	 */

	struct stat ss = { 0 };
	if (stat(DB_PATH, &ss) != 0) {
		/* Stores the errno that stat set, stops fprintf or other library calls overriding it. */
		register int snapshot = errno;
		/* Understand the error that stat threw. */
		if (snapshot == ENOENT) {
			/* This means that no file or directory is present, consequently make the directory. */
			if (handle_dir(DB_PATH) != 0) {
				return -1;
			}
		}
	} else {
		/* If it is a directory and does exist set the permissions using chmod. */
		if (S_ISDIR(ss.st_mode)) {
			chmod(DB_PATH, dir_perms);
		} else {
			rename(DB_PATH, DB_PATH ".old");
			if (handle_dir(DB_PATH) != 0) {
				return -1;
			}
		}
	}
	fprintf(stdout, "sspkg-boostrap: validated directory %s is present, a directory and has correct permissions.\n", DB_PATH);

	return 0;
}
