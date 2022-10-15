#include "dir.h"
#include "match.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Callback which will be called for each file
 */

static void file_cb(const char *dirname, const char *fname, void *cxt)
{
	if (match(fname, (const char *) cxt) == 0)
		printf("%s%s\n", dirname, fname);
}

int main(int argc, char **argv)
{
	const char *default_dir = ".";
    const char *startdir = NULL;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s filename [dirname]\n"
						"filename can consist of special characters:\n"
						"\t* - one or more any characters\n"
						"\t? - one any character\n", argv[0]);
		return 1;
	}

	/*
	 * Choosing start poing 
	 */

	startdir = (argv[2]) ? argv[2] : default_dir;

	/*
     * Scaning all directories calling file_cb for each file
	 */

	if (dir_scan(startdir, file_cb, argv[1]) < 0)
	{
		ERROR("ERROR");
		return 1;
	}

	return 0;
}
