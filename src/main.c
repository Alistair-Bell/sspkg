#include <getopt.h>

#include "inc.h"
#include "db.h"
#include "bootstrap.h"

/* Defines the format in which getopt will parse arguments, also used in `args_msg_help` bsd style. */
static const char *msg_opts = "Bhilrv";

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
				
			}
			case 'l': {

			}
			case 'r': {

			}
			case 'v': {

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
