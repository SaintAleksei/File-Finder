#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED 1

#include <errno.h>
#include <stdio.h>
#include <string.h>

/*
 * Simple macro for error printing
 * It's like 'perror', but this macro can work with format string
 */

#define ERROR(...)                             \
do                                             \
{                                              \
    fprintf(stderr, __VA_ARGS__);              \
    fprintf(stderr, ": %s\n", strerror(errno));\
}                                              \
while(0)                                       

#endif
