#include "db.h"

#define STR_BUFF 128

int8_t 
db_open(struct db *db, const char *name, const char *md)
{
	/* Store the name in the db struct, only used for messaging. */
	db->name = name;

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
	int8_t res = (strcmp(md, "rb") == 0) ? perms_valid_read(buff) : perms_valid_write(buff);

	/* Handle the result. */
	switch (res) {
		case -1: {
			fprintf(stdout, "sspkg: error unable to find database file %s, does it exist?\n", buff);
			return -1;
		}
		case -2: {
			fprintf(stdout, "sspkg: error specified database is a directory, a file is required.\n");
			return -1;
		}
		case -3: {
			fprintf(stdout, "sspkg: error unable to retrive current uid groups, cannot validate correct permissions.\n");
			return -1;
		}
		case -4: {
			fprintf(stdout, "sspkg: error cannot read/write to database %s, all permission checks failed, check you have correct access rights!\n", buff);
			return -1;
		}
		/* All perms are correct, carry on with operation. */
		default: {
		}
	}

	db->ref = fopen(buff, md);
	return 0;
}
int8_t
db_read(struct db *db, struct pkg **load, uint32_t *count)
{
	register struct db_header *h = &db->header;

	/* The header of the database tells the loader how many packages to load. */
	rewind(db->ref);
	fread(h, sizeof(*h), 1, db->ref);
	*count = h->pkg_count;
	
	/* Check the validity of the database. */
	if (strcmp(db->header.magic, DB_MAGIC_BYTES) != 0) {
		fprintf(stderr, "sspkg: error database %s is not a database file, invalid magic 8 bytes, data may be corrupted!\n", db->name);
		return -1;
	}
	
	/* Return if zero packages are present. */
	if (h->pkg_count <= 0) {
		fprintf(stderr, "sspkg: warning database %s has no packages.\n", db->name);
		return 0;
	}

	/* Allocate the required memory that the packages require. */
	*load = calloc(*count, sizeof(*(*load)));
	fseek(db->ref, sizeof(*h), SEEK_SET);
	fread(*load, sizeof(*(*load)) * h->pkg_count, 1, db->ref);
	return 0;
}
int8_t
db_write(struct db *db, struct pkg *write, uint32_t count)
{
	/* This function assumes that it is in rb mode. */

	/* Format the header. */
	struct db_header *h = &db->header;
	strncpy(h->magic, DB_MAGIC_BYTES, 8);
	h->ver       = DB_FORMAT_VER;
	h->pkg_count = count;

	size_t res = 0;
	/* Firstly write the header. */
	res += fwrite(h, sizeof(*h), 1, db->ref);

	/* Write the packages, offsetting the stream after the header. */
	fseek(db->ref, sizeof(*h), SEEK_SET);

	res += fwrite(write, sizeof(uint8_t), sizeof(*write) * count, db->ref);
	
	if (res == 0) {
		fprintf(stdout, "sspkg: error unable to write to database %s, writing errors have occured, code %d!\n", db->name, ferror(db->ref));
	}
	return 1;
}
void 
db_close(struct db *db)
{
	if (db->ref != NULL) {
		fclose(db->ref);
	}
}


