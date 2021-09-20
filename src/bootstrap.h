#ifndef SSPKG_BOOTSTRAP_H
#define SSPKG_BOOTSTRAP_H

#include "inc.h"
#include "db.h"
#include "perms.h"

/* Runs a bootstrap procedure, this sets permissions creates directories and fetches repo files. */
int8_t bootstrap(void);
/* Validates all the permissions and files are valid and present, can be used to recover a broken package manager. */
int8_t validate_bootstrap(void);


#endif
