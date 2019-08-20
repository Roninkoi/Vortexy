#include <stdlib.h>
#include <string.h>

#include "util.h"

// str is dynamically allocated and is freed
inline char *strAppend(char *str, char c)
{
	int len = 0;

	if (str != NULL) {
		len = strlen(str);
	}

	char *newStr = calloc(len + 1, sizeof(char));

	if (len > 0) {
		strcpy(newStr, str);
	}

	newStr[len] = c;

	free(str);

	return newStr;
}

// dest is freed
inline char *strConcat(char *dest, char *src)
{
	int destLen = strlen(dest);
	int srcLen = strlen(src);

	char *newStr = calloc(destLen + srcLen, sizeof(char));

	strcpy(newStr, dest);
	strcat(newStr, src);

	free(dest);

	return newStr;
}

// arr is freed
inline float *floatAppend(float *arr, float c, int n)
{
	float *newArr = malloc((n + 1) * sizeof(float));

	memcpy(newArr, arr, n * sizeof(float));

	newArr[n] = c;

	free(arr);

	return newArr;
}

inline float *floatCopy(float *arr, int n)
{
	float *newArr = malloc(n * sizeof(float));

	memcpy(newArr, arr, n * sizeof(float));

	return newArr;
}

// arr is freed
inline int *intAppend(int *arr, int c, int n)
{
	int *newArr = malloc((n + 1) * sizeof(int));

	memcpy(newArr, arr, n * sizeof(int));

	newArr[n] = c;

	free(arr);

	return newArr;
}

inline int **intsAppend(int **arr, int *c, int n)
{
	int **newArr = malloc((n + 1) * sizeof(int *));

	memcpy(newArr, arr, n * sizeof(int *));

	newArr[n] = c;

	free(arr);

	return newArr;
}

inline int contains(int *arr, int c, int n)
{
	for (int i = 0; i < n; ++i) {
		if (arr[i] == c) {
			return 1;
		}
	}

	return 0;
}
