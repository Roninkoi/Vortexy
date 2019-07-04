#ifndef TIME_H
#define TIME_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

unsigned int timeNow()
{
    return time(NULL);
}

const char *currentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    int size = 80 * sizeof(char);
    char *buf = malloc(size);
    tstruct = *localtime(&now);
    strftime(buf, size, "%Y-%m-%d.%X", &tstruct);

    return buf;
}

#endif
