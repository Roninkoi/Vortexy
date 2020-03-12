#ifndef TIME_H
#define TIME_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static long timeMillis()
{
	long t;
	struct timespec spec;

	clock_gettime(CLOCK_MONOTONIC, &spec);
	//timespec_get(&spec, TIME_UTC);

	t = round(spec.tv_nsec / 1.0e6) + spec.tv_sec * 1000.0;

	return t;
}

unsigned int timeNow()
{
	return time(NULL);
}

char *currentDateTime()
{
	time_t now = timeNow();
	struct tm tstruct;

	int size = 80 * sizeof(char);

	char *buf = malloc(size);
	tstruct = *localtime(&now);

	strftime(buf, size, "%d.%m.%Y %X", &tstruct);

	return buf;
}

#endif
