#include "match.h"

#include <stdio.h>
#include <string.h>

int match(const char *fname, const char *pattern)
{
    if (!fname)
        return -1;

    while (*fname && *pattern)
    {
        if (*pattern == '*')
        {
            fname++;
            if (*fname == *(pattern + 1))
                pattern++;
        }
        else if (*pattern == '?' || *fname == *pattern)
        {
            pattern++;
            fname++;
        }
        else
        {
            break;
        }
    }

    return *fname - *pattern;
}
