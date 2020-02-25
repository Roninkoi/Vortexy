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
	int cvi = 0;
	int cpi = 0;
	int ici = 0;
	int ipi = 0;

	for (int i = 0; i < wordNum; ++i) {
		if (i + 1 < wordNum) {
			if (strcmp(words[i], "mu") == 0) {
				o->fluid.mu = atof(words[i + 1]);
			}
			if (strcmp(words[i], "nu") == 0) {
				o->fluid.nu = atof(words[i + 1]);
			}
			if (strcmp(words[i], "mesh") == 0) {
				o->meshPath = calloc(strlen(words[i + 1]), sizeof(char));
				strncpy(o->meshPath, words[i + 1], strlen(words[i + 1]));

				p_loadObjMesh(o);
			}
			if (strcmp(words[i], "rho") == 0) {
				o->fluid.rho = atof(words[i + 1]);
			}
			if (strcmp(words[i], "p") == 0) {
				o->fluid.p0 = atof(words[i + 1]);
			}
			if (strcmp(words[i], "bc") == 0) {
				bci = atoi(words[i + 1]);
				o->faces[bci].boundary = atoi(words[i + 2]);
			}
			if (strcmp(words[i], "cv") == 0) {
				cvi = atoi(words[i + 1]);
				o->faces[cvi].constantV = atof(words[i + 2]);
			}
			if (strcmp(words[i], "cp") == 0) {
				cpi = atoi(words[i + 1]);
				o->faces[cvi].constantP = atof(words[i + 2]);
			}
			if (strcmp(words[i], "iv") == 0) {
				ici = atoi(words[i + 1]);
				o->faces[ici].initialV = atof(words[i + 2]);
			}
			if (strcmp(words[i], "ip") == 0) {
				ipi = atoi(words[i + 1]);
				o->faces[ipi].initialP = atof(words[i + 2]);
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
			if (strcmp(words[i], "simulating") == 0) {
				s->sys.simulating = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "dt") == 0) {
				s->sys.dt = atof(words[i + 1]);
			}
			if (strcmp(words[i], "endt") == 0) {
				s->sys.endt = atof(words[i + 1]);
			}
			if (strcmp(words[i], "epsilon") == 0) {
				s->sys.epsilon = atof(words[i + 1]);
			}
			if (strcmp(words[i], "pr") == 0) {
				s->sys.pRelax = atof(words[i + 1]);
			}
			if (strcmp(words[i], "vr") == 0) {
				s->sys.vRelax = atof(words[i + 1]);
			}
			if (strcmp(words[i], "maxit") == 0) {
				s->sys.maxIt = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "outputting") == 0) {
				s->outputting = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "fluid") == 0) {
				s->fluidPath = calloc(strlen(words[i + 1]), sizeof(char));
				strncpy(s->fluidPath, words[i + 1], strlen(words[i + 1]));
			}
			if (strcmp(words[i], "output") == 0) {
				s->outPath = calloc(strlen(words[i + 1]), sizeof(char));
				strncpy(s->outPath, words[i + 1], strlen(words[i + 1]));
			}
			/*if (strcmp(words[i], "cz") == 0) {
				s->renderer.cz = atof(words[i + 1]);
			}*/
		}
	}
	
	for (int i = 0; i < wordNum; ++i)
		free(words[i]);

	free(words);
}

#endif
