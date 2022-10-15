#ifndef DIR_H_INCLUDED
#define DIR_H_INCLUDED 1

#ifndef NAME_SIZE
#define NAME_SIZE 0x400U
#endif

/*
 * This function call file_cb for each file in start directory and subdirectories
 */

int dir_scan(const char *start, 
             void (*file_cb)(const char *dirname, const char *fname, void *cxt), 
             void *cxt);

#endif
