#include "perms.h"

int8_t
perms_valid(struct stat *sbuff, const uint16_t owm, const uint16_t grm, const uint16_t otm)
{
	/* Handy var. */
	register mode_t m = sbuff->st_mode;

	/* Firstly validate that others can access using the mask. */
	if (~m & otm) {
		/* Other user does not have correct perms. */
		uid_t uid = getuid();

		/* Check that the uid of the current uid owns the file and has perms. */
		if (uid == sbuff->st_uid && m & owm) {
			return 0;	
		}

		/* Last resort check common groups with current uid. */
		struct passwd *pw = getpwuid(uid);
		if (pw == NULL) {
			return -2;
		}
		
		/* Returns -3 on all checks have failed. */
		return (pw->pw_gid == sbuff->st_gid && m & grm) ? 0 : -3;
	}
	return 0;
}
