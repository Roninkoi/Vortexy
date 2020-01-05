#ifndef UTIL_H
#define UTIL_H

typedef unsigned char byte;

typedef enum {
	false, true
} bool;

#define min(a, b) (a < b ? a : b)

#define max(a, b) (a > b ? a : b)

#define signpowf(n) ((n) % 2 == 0 ? 1.0f : -1.0f)

// str is dynamically allocated and is freed
extern char *strAppend(char *str, char c);

// dest is freed
extern char *strConcat(char *dest, char *src);

// arr is freed
extern float *floatAppend(float *arr, float c, int n);

extern float *floatCopy(float *arr, int n);

// arr is freed
extern int *intAppend(int *arr, int c, int n);

extern int **intsAppend(int **arr, int *c, int n);

extern int contains(int *arr, int c, int n);

extern char **wordsFromFile(char *path, int size, int *wordNum);

#endif
