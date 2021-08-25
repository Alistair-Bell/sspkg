/* 
 * This program is intended as a way to hard generate repo files, USED FOR DEV ONLY.
 */

#include "inc.h"
#include "db.h"

#define REPO "master"

int
main(void)
{
	struct pkg pkgs[] = {
		(struct pkg) {
			.name = "sspkg",
			.desc = "Simple source package manager.",
			.author = "Alistair Bell",
			.license = 1,
			.arch = 1,
			.ver = 1,
		},
		(struct pkg) {
			.name = "holyc",
			.desc = "HolyC for modern unix systems.",
			.author = "Alistair Bell",
			.license = 1,
			.arch = 1,
			.ver = 1,
		},
	};

	fprintf(stdout, "sspkg: warning bootstraping will destroy all previous repo files!\n");	

	/* Open the database in write binary mode. */
	struct db d = { 0 };

	if (db_open(&d, REPO, "wb") < 0) {
		return 0;
	}
	/* Write the data*/
	db_write(&d, pkgs, 2);
	db_close(&d);
}
