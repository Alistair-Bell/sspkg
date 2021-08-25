#ifndef SS_INC_H
#define SS_INC_H

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <pwd.h>

/* Macro compatibility with other systems. */
#ifndef S_IREAD
	#define S_IREAD S_IRUSR
#endif

#ifndef S_IWRITE
	#define S_IWRITE S_IWUSR
#endif

#endif
