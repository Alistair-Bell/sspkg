#ifndef SS_ARGS_H
#define SS_ARGS_H

#include "inc.h"

/* Flags for the target operation */
enum args_operation {
	/* If the data operation flag is none after the `args_parse` call then the program is wrongly called. */
	ARGS_OPERATION_NONE,
	ARGS_OPERATION_INSTALL,
	ARGS_OPERATION_REMOVE,
};

struct args_data {
	const char **pkgs;
	uint16_t pkgs_count;
	enum args_operation operation;
};


/* Parses the arguments passed to the program, populates `struct args_data`. Returns -1 for exit, -2 for display help then exit. */
int8_t args_parse(int32_t, const char *[], struct args_data *);

/* Non function specific mesages written stdout, triggered by passed flag. */
void args_msg_help(void);
void args_msg_version(void);
void args_msg_license(void);

#endif
