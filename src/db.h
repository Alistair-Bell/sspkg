#ifndef SSPKG_DB_H
#define SSPKG_DB_H

#include "inc.h"

/* 
 * Because the database file is unlikely to be modified by hand, the file is a binary serialisation of the pkg struct.
 * It uses a custom format I call sspkg-db, pretty snazzy.
 * The magic bytes are 8 bytes long and spells SS PK GD B\0.
 * This header and source provides essential database and functionalit.
 */


/* Required starting magic. */
#define DB_MAGIC_BYTES 0x5353504b4744420



/* Path where files are stored. This is different depending on os */
#if CONFIG_BSD
	#define DB_PATH "/var/db/sspkg-db/"
	#define DB_PATH_LEN 17
#else
	#define DB_PATH "/var/lib/sspkg-db/"
	#define DB_PATH_LEN 18
#endif


/* Describes the first X bytes of the database file. */
struct db_header {
	/* Magic 8 bytes used to identify the version. */
	uint64_t magic;
	/* Revision of the binary format. */
	uint32_t format_ver;
	/* Amount of packages contained. */
	uint32_t pkg_count;
};

struct db {
	const char       *name;
	FILE             *ref;
	struct db_header header;
};

/* All string stored within the format must be null terminated 8 byte chars */
struct pkg {
	const int8_t *name;
	const int8_t *author;
	const int8_t *build;
	uint16_t     license;
	uint8_t      arch; 
	uint32_t     ver;
};

int8_t db_open(const char *, struct db *);
uint32_t db_load(struct db *, struct pkg **);
void db_close(struct db *);

#endif
