#ifndef SSPKG_DB_H
#define SSPKG_DB_H

#include "inc.h"
#include "perms.h"

/* 
 * Because the database file is unlikely to be modified by hand, the file is a binary serialisation of the pkg struct.
 * It uses a custom format I call sspkg-db, pretty snazzy.
 * The magic bytes are 8 bytes long and spells SS PK GD B\0.
 * This header and source provides essential database and functionalit.
 */


/* Required starting magic. */
#define DB_MAGIC_BYTES "SSPKG"
/* Current format revision. */
#define DB_FORMAT_VER 1



/* Path where files are stored. This is different depending on os */
#if CONFIG_BSD
	#define DB_PATH "/var/db/sspkg-db"
	#define DB_PATH_LEN 16
#else
	#define DB_PATH "/var/lib/sspkg-db"
	#define DB_PATH_LEN 17
#endif


/* Describes the first X bytes of the database file. */
struct db_header {
	/* Magic 8 bytes used to identify the version. */
	char magic[8];
	/* Revision of the binary format. */
	uint32_t ver;
	/* Amount of packages contained. */
	uint32_t pkg_count;
};

struct db {
	const char       *name;
	FILE             *ref;
	struct db_header header;
};

struct pkg {
	uint32_t     ver;
	uint16_t     license;
	uint16_t     arch;
	char         name[15];
	char         desc[30];
	char         author[30];
};

int8_t db_open(struct db *, const char *, const char *);
int8_t db_read(struct db *, struct pkg **, uint32_t *);
int8_t db_write(struct db *, struct pkg *, uint32_t);
void db_close(struct db *);

#endif
