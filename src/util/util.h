#ifndef UTIL_H
#define UTIL_H

typedef unsigned char byte;

typedef enum {
	false, true
} bool;

// str is dynamically allocated and is freed
extern char *strAppend(char *str, char c);

// dest is freed
extern char *strConcat(char *dest, char *src);

// arr is freed
extern float *floatAppend(float *arr, float c, int n);

extern float *floatCopy(float *arr, int n);

// arr is freed
extern int *intAppend(int *arr, int c, int n);

#endif
