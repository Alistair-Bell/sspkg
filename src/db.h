#ifndef SSPKG_DB_H
#define SSPKG_DB_H

#include "inc.h"
#include "perms.h"

/* Required starting magic. */
#define DB_MAGIC_BYTES "SSPKG_DB"
/* Current format revision. */
#define DB_FORMAT_VER 1

/* Path where files are stored. This is different depending on os, based on `man heir`. */
#if CONFIG_BSD
	#define DB_PATH "/var/db/sspkg-db"
	#define DB_PATH_LEN 16
#else
	#define DB_PATH "/var/lib/sspkg-db"
	#define DB_PATH_LEN 17
#endif


/* Describes the layout of the database file. */
struct db_header {
	char magic[8];
	uint32_t ver;
	uint32_t pkg_count;
};

/* Object handle to the database. */
struct db {
	struct db_header header;
	const char       *name;
	FILE             *ref;
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
void   db_close(struct db *);

#endif
