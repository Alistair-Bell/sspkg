#include "db.h"

#define BUFF_SIZE 128

int8_t
db_open(struct db *db, const char *name, const char *mode)
{
	char path_buffer[BUFF_SIZE];
	if (BUFF_SIZE <= DB_PATH_LEN + strlen(name)) {
		fprintf(stderr, "sspkg: error database path buffer size exceeded, %s must be a smaller string.\n", name);
		return -1;
	}

	sprintf(path_buffer, "%s/%s", DB_PATH, name);
	path_buffer[DB_PATH_LEN + strlen(name) + 1] = '\0';
	
	/* Call stat. */
	struct stat sbuff;
	if (stat(path_buffer, &sbuff) < 0) {
		register int snapshot = errno;
		fprintf(stderr, "sspkg: error failed to stat database file %s, error code %d.\n", path_buffer, snapshot);
		return -1;
	}

	int8_t validate_result;
	if (strcmp(mode, "rb") == 0) {
		validate_result = perms_valid_read(&sbuff);
	} else {
		validate_result = perms_valid_write(&sbuff);
	}
	
	if (validate_result < 0) {
		fprintf(stderr, "sspkg: error failed to validate that user %d has correct permissions for mode \'%s\' on database %s!\n", getuid(), mode, path_buffer);
		return -1;
	}

	/* Call fopen on the file. */
	db->ref  = fopen(path_buffer, mode);
	db->name = name;

	return 0;
}
int8_t
db_read(struct db *db, struct pkg **out, uint32_t *count)
{
	/* Firstly read the header, this can validate the data that is within the file using maths. */
	fseek(db->ref, 0l, SEEK_END);
	size_t size = ftell(db->ref);

	/* Firstly validate that the header size is correct. */
	if (size < sizeof(db->header)) {
		fprintf(stderr, "sspkg: error database %s lacks data, file corrupted or incomplete, run bootstrap to restore!\n", db->name);
		return -1;
	}

	/* Read the database header into the db->header member. */
	rewind(db->ref);
	fread(&db->header, 1, sizeof(db->header), db->ref);
	
	/* Validate the header. */
	if (strncmp(db->header.magic, DB_MAGIC_BYTES, 7) != 0) {
		fprintf(stderr, "sspkg: error database %s's magic bytes are incorrect, file may be damaged or corrupted, run boostrap to restore!\n", db->name);
		return -1;
	}

	/* Use maths to validate the package count matches the amount of bytes within the files, invalid amount of bytes may mean corruption. */
	if ((db->header.count * sizeof(struct pkg)) + sizeof(db->header) != size) {
		fprintf(stderr, "sspkg: error database %s has invalid byte count, data may be damaged or corrupted, run boostrap to restore!\n", db->name);
		return -1;
	}

	/* When all checks are correct allcate the `out` memory and set the `count` variable. Read the package data. */
	*count  = db->header.count;
	*out    = (struct pkg *)malloc(sizeof(struct pkg) * (*count));
	/* Seek past the database header. */
	fseek(db->ref, sizeof(db->header), SEEK_SET);
	fread((struct pkg *)*out, sizeof(struct pkg), *count, db->ref);
	return 0;
}
int8_t
db_write(struct db *db, struct pkg *in, uint32_t count)
{
	return -1;
}
void
db_close(struct db *db)
{
	fclose(db->ref);
}


