#ifndef RANDOM_H
#define RANDOM_H

#include <util/timeUtil.h>
#include <stdlib.h>

float randomFloat()
{
    int r = rand(); // not very random

    float fr = (float) r / (float) RAND_MAX;

    return fr;
}

int randomRange(int min, int max)
{
    float fr = randomFloat();

    int rr = (int)(fr * (float) (max - min) + min);

    return rr;
}

#endif
