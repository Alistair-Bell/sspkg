#include <getopt.h>

#include "inc.h"
#include "db.h"
#include "bootstrap.h"
#include "licenses.h"

/* Defines the format in which getopt will parse arguments, also used in `args_msg_help` bsd style. */
static const char *msg_opts = "Bhi:lr:v";
static const char *target_db = "master";
static struct db database = { 0 };

static int8_t
option_install(const char *arg)
{
	/* Open the database in readmode . */
	if (db_open(&database, target_db, "rb") < 0) {
		return -1;
	}
	uint32_t count;
	struct pkg *packages;
	if (db_read(&database, &packages, &count) < 0) {
		return -1;
	}
	db_close(&database);
	free(packages);
	return 0;
}
static int8_t
option_list(void)
{
	/* Open the database in read mode. */	
	if (db_open(&database, target_db, "rb") < 0) {
		return -1;
	}
	uint32_t count; struct pkg *packages;
	if (db_read(&database, &packages, &count) < 0) {
		return -1;
	}
	fprintf(stdout, "sspkg: database %s yields %u package(s).\n\n", database.name, count);
	/* This prints the package in reverse. */
	while (count != 0) {
		struct pkg *ref = &packages[count - 1];
		fprintf(stdout, "name:    %s\ndesc:    %s\nauthor:  %s\nversion: %d.%d.%d\n", ref->name, ref->desc, ref->author, VER_MAJOR(ref->ver), VER_MINOR(ref->ver), VER_PATCH(ref->ver));
		/* Print the license, validate that it is has a matching value within the index. */
		if (ref->license < ARRAY_LEN(license_table)) {
			/* Print out the license from the table and then the version. */
			fprintf(stdout, "license: %s revision %d.%d\n", license_table[ref->license], ref->license_ver & 0xff, (ref->license_ver & 0xff00) >> 8);
		} else {
			/* Display license as unknown. */
			fprintf(stdout, "license: unknown, not resolvable\n");
		}
		--count;
	}
	free(packages);
	return 0;
}

static void
args_msg_help(void)
{
	fprintf(stdout, "sspkg: [-%s] [-B boostrap] [-h help] [-i install] [-l list] [-r remove] [-v version] [package(s)].\n", msg_opts);
}

static int32_t
args_parse(int32_t argc, char *const *argv)
{
	/* Set opterr not to print default error messages. */
	opterr = 0;
	int opt;
	while ((opt = getopt(argc, argv, msg_opts)) != -1) {
		switch ((char)opt) {
			case 'B': {
				return bootstrap();
			}
			case 'h': {
				return -2;
			}
			case 'i': {
				option_install(optarg);
				break;
			}
			case 'l': {
				return option_list();
			}
			case 'r': {
				break;
			}
			case 'v': {
				break;
			}
			default: {
				fprintf(stderr, "sspkg: error unknown or invalid usage of flag -%c, refer to -h for program usage.\n", optopt);
				return -1;
			}
		}
	}
	return 0;
}

int
main(int argc, const char *argv[])
{
	/* Check for a single argument. */
	if (argc < 2) {
		args_msg_help();
		return 1;
	}

	/* Returns < 0 mean that the program should terminate. */
	int32_t result;
	if ((result = args_parse(argc, (char *const *)argv)) < 0) {
		/* Displays a help message. */
		if (result == -2) {
			args_msg_help();
			return 1;
		}
		return 1;
	}

	return 0;
}
