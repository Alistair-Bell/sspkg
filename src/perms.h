#ifndef SSPKG_PERMS_H
#define SSPKG_PERMS_H

#include "inc.h"

/* Macro for read with predefined bit masks. */
#define perms_valid_read(x) perms_valid(x, S_IREAD, S_IRGRP, S_IROTH)
/* Macro for write with predefined bit masks. */
#define perms_valid_write(x) perms_valid(x, S_IWRITE, S_IWGRP, S_IWOTH)

/* Checks if the runner has access to the file, uses bitmasks. */
int8_t perms_valid(const char *, const uint16_t, const uint16_t, const uint16_t);

#endif
