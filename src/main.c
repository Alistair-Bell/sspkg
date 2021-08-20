#include "inc.h"
#include "args.h"

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

	free(op.pkgs);
}
