#ifndef SSPKG_INC_H
#define SSPKG_INC_H

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

#define GEN_VER(major, minor, patch) \
	(((uint32_t)(major)) << 22) | (((uint32_t)(minor)) << 12) | (((uint32_t)(patch)))

#define VER_MAJOR(val) \
	((uint32_t)(val) >> 12) & 0x3ff

#define VER_MINOR(val) \
	((uint32_t)(val) >> 22) & 0x3ff

#define VER_PATCH(val) \
	((uint32_t)(val) & 0xfff)

#endif
