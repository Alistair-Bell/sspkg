#include "inc.h"
#include "args.h"
#include "db.h"

static const char *db_name = "master";

static int8_t
pkg_install(struct args_data *args)
{
	struct db d = { 0 };
	if (db_open(&d, db_name, "rb") < 0) {
		return -1;
	}

	db_close(&d);
	return -1;
}


int
main(int argc, const char *argv[])
{
	/* Used to determine the operation that is required. */
	struct args_data op = { 0 };

	/* Check for a single argument. */
	if (argc < 2) {
		args_msg_help();
		return 1;
	}

	/* Returns < 0 mean that the program should terminate. */
	int32_t result;
	if ((result = args_parse(argc, argv, &op)) < 0) {
		/* Displays a help message. */
		if (result == -2) {
			args_msg_help();
			return 1;
		}
		return 1;
	}

	switch (op.operation) {
		case ARGS_OPERATION_INSTALL: {
			pkg_install(&op);
		}
		default: {
			break;
		}
	}

	free(op.pkgs);
	return 0;
}
