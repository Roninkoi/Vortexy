#include <stdlib.h>
#include <stdio.h>
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

inline char **wordsFromFile(char *path, int size, int *wordNum)
{
	FILE *fp;
	char *data;
	size_t fsize;

	fp = fopen(path, "r");

	if (!fp) {
		printf("Not found: %s\n", path);
		exit(1);
	}

	data = calloc(size, sizeof(char *));
	fsize = fread(data, 1, size, fp);

	fclose(fp);

	char **words = calloc(size, sizeof(char *));

	for (int i = 0; i < size; ++i) {
		words[i] = calloc(1, sizeof(char));
	}

	int wn = 0;
	int comment = 0;

	for (int i = 0; data[i]; ++i) { // separate into words
		char read = data[i];

		if (comment && (read == '\n' || read == '\r')) {
			comment = 0;
		}
		
		if (read == '#') {
			comment = 1;
		}
		
		if (comment) {
			continue;
		}

		if (read != ' ' && read != '\n' && read != '\r') {
			words[wn] = strAppend(words[wn], read);
		}
		else {
			++wn;
		}
	}

	free(data);

	*wordNum = wn;
	
	return words;
}
