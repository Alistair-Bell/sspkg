#include "perms.h"

int8_t
perms_valid(const char *path, const uint16_t owm, const uint16_t grm, const uint16_t otm)
{
	struct stat s;
	/* Validate that stat actually worked. */
	if (stat(path, &s) != 0) {
		return -1;
	}

	/* Handy var. */
	register mode_t m = s.st_mode;

	/* Firstly validate that others can access using the mask. */
	if (~m & otm) {
		/* Other user does not have correct perms. */
		uid_t uid = getuid();

		/* Check that the uid of the current uid owns the file and has perms. */
		if (uid == s.st_uid && m & owm) {
			return 0;	
		}

		/* Last resort check common groups with current uid. */
		struct passwd *pw = getpwuid(uid);
		if (pw == NULL) {
			return -2;
		}
		
		/* Returns -3 on all checks have failed. */
		return (pw->pw_gid == s.st_gid && m & grm) ? 0 : -3;
	}
	return 0;
}
