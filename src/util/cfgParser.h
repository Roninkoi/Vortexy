#ifndef CFG_PARSER_H
#define CFG_PARSER_H

#include <string.h>

#include "phys/fluid.h"
#include "sim.h"
#include "util/util.h"

#define CFG_MAX 0x100000

void fluidParser(Obj *o, char *path)
{
	int wordNum = 0;
	char **words = wordsFromFile(path, CFG_MAX, &wordNum);

	int bci = 0;
	int cfi = 0;
	int ici = 0;

	for (int i = 0; i < wordNum; ++i) {
		if (i + 1 < wordNum) {
			if (strcmp(words[i], "mu") == 0) {
				o->fluid.mu = atof(words[i + 1]);
			}
			if (strcmp(words[i], "nu") == 0) {
				o->fluid.nu = atof(words[i + 1]);
			}
			if (strcmp(words[i], "rho") == 0) {
				o->fluid.rho = atof(words[i + 1]);
			}

			if (strcmp(words[i], "bc") == 0 && bci < o->faceNum) {
				o->faces[bci].boundary = atoi(words[i + 1]);
				++bci;
			}
			if (strcmp(words[i], "cf") == 0 && cfi < o->faceNum) {
				o->faces[cfi].constant = atof(words[i + 1]);
				++cfi;
			}
			if (strcmp(words[i], "ic") == 0 && ici < o->faceNum) {
				o->faces[ici].initial = atof(words[i + 1]);
				++ici;
			}
		}
	}
	
	for (int i = 0; i < wordNum; ++i)
		free(words[i]);

	free(words);
}

void simParser(struct Sim *s, char *path)
{
	int wordNum = 0;
	char **words = wordsFromFile(path, CFG_MAX, &wordNum);

	for (int i = 0; i < wordNum; ++i) {
		if (i + 1 < wordNum) {
			if (strcmp(words[i], "render") == 0) {
				s->rendered = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "gpu") == 0) {
				s->usegpu = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "cldevice") == 0) {
				s->device = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "dt") == 0) {
				s->dt = atof(words[i + 1]);
			}
			if (strcmp(words[i], "fluid") == 0) {
				s->fluidPath = calloc(strlen(words[i + 1]), sizeof(char));
				strncpy(s->fluidPath, words[i + 1], strlen(words[i + 1]));
			}
			if (strcmp(words[i], "mesh") == 0) {
				s->meshPath = calloc(strlen(words[i + 1]), sizeof(char));
				strncpy(s->meshPath, words[i + 1], strlen(words[i + 1]));
			}
			if (strcmp(words[i], "output") == 0) {
				s->outPath = calloc(strlen(words[i + 1]), sizeof(char));
				strncpy(s->outPath, words[i + 1], strlen(words[i + 1]));
			}
		}
	}
	
	for (int i = 0; i < wordNum; ++i)
		free(words[i]);

	free(words);
}

#endif
