#include "db.h"

#define STR_BUFF 128

/* Validates the format of the file. */
static int8_t
file_validation(char *buff, struct db *db)
{
	/* Check it exists. */
	struct stat s;
	if (stat(buff, &s) != 0) {
		fprintf(stderr, "sspkg: error unable to find database file %s, does it exist!\n", buff);	
		return -1;
	}
	
	/* Check that it has at least X bytes. */
	if (s.st_size < (off_t)sizeof(db->header)) {
		fprintf(stderr, "sspkg: error database file %s header is too small, file may be corrupted or incomplete.\n", buff);
		return -1;
	}
	
	/* Firstly validate whether any user can acess the database file. */
	if (~s.st_mode & S_IROTH) {
		uid_t uid = getuid();
		
		/* Check that the running uid and file uid are the same, also check the current uid has read perms. */
		if ((uid == s.st_uid) && (s.st_uid & S_IREAD)) {
			goto read_file;
		}
		
		/* Last resort check that the uid is in a readable group for the file. */
		struct passwd *pw = getpwuid(uid);
		if (pw == NULL) {
			fprintf(stderr, "sspkg: error failed to get passwd for uid, other read checks failed, cannot open database %s.\n", buff);
			return -1;
		}

		if ((pw->pw_gid == s.st_gid) && (s.st_uid & S_IRGRP)) {
			goto read_file;
		}
		fprintf(stderr, "sspkg: read permission checks all failed for database %s, make sure you execute sspkg with database read permissions.\n", buff);
		return -1;
	}

read_file:

	/* Open the file for reading. */
	db->ref = fopen(buff, "rb");

	/* Set the read position to the start of the file. */
	rewind(db->ref);

	fread((void *)&db->header, sizeof(db->header), 1, db->ref);

	fprintf(stdout, "sspkg: database header magic %ld\n", db->header.magic);

	fclose(db->ref);
	return 0;
}

int8_t db_open(const char *name, struct db *db)
{
	/* Format the lookup for the file. */
	char buff[STR_BUFF + 1] = { 0 };

	/* As this is being run as uid 0 then overflow exploits are crucial to not occur. */
	uint32_t size = strlen(name) + DB_PATH_LEN;
	if (STR_BUFF < size) {
		fprintf(stderr, "sspkg: error database name %s is too long, please shorten the name to less than %d chars!\n", name, STR_BUFF);
		return -1;
	}

	/* Join the 2 strings together. */
	sprintf(buff, "%s%s", DB_PATH, name);
	/* Set the null terminator at the end of the 2 strings. */
	buff[size] = '\0';

	/* Validate the file supplied is valid. */
	if (file_validation(buff, db) < 0) {
		return -1;
	}
	
	return 0;
}
void db_close(struct db *db)
{

}


