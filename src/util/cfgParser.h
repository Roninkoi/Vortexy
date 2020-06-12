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
			if (strcmp(words[i], "end") == 0) {
				break;
			}
			if (strcmp(words[i], "mu") == 0) {
				o->fluid.mu = atof(words[i + 1]);
				printf("mu %f\n", o->fluid.mu);
			}
			if (strcmp(words[i], "rho") == 0) {
				o->fluid.rho = atof(words[i + 1]);
				printf("rho %f\n", o->fluid.rho);
			}
			if (strcmp(words[i], "dt") == 0) {
				o->dt = atof(words[i + 1]);

				printf("dt %f\n", o->dt);
			}
			if (strcmp(words[i], "pr") == 0) {
				o->pRelax = atof(words[i + 1]);
			}
			if (strcmp(words[i], "vr") == 0) {
				o->vRelax = atof(words[i + 1]);
			}
			if (strcmp(words[i], "f") == 0) {
				o->fluid.f = Vec3(atof(words[i + 1]), atof(words[i + 2]), atof(words[i + 3]));
				
				printf("f ");
				vec3Print(&o->fluid.f);
			}
			if (strcmp(words[i], "bp") == 0) {
				o->fluid.bp = atof(words[i + 1]);
			}
			if (strcmp(words[i], "ebc") == 0) {
				o->fluid.ebc = atoi(words[i + 1]);
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
				o->faces[cpi].constantP = atof(words[i + 2]);
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
	
	freeStrArr(words, CFG_MAX);
}

void fluidMeshParser(Obj *o, char *path)
{
	int wordNum = 0;
	char **words = wordsFromFile(path, CFG_MAX, &wordNum);

	for (int i = 0; i < wordNum; ++i) {
		if (i + 1 < wordNum) {
			if (strcmp(words[i], "end") == 0) {
				break;
			}
			if (strcmp(words[i], "mesh") == 0) {
				o->meshPath = calloc(strlen(words[i + 1])+1, sizeof(char));
				strncpy(o->meshPath, words[i + 1], strlen(words[i + 1]));
			}
		}
	}
	
	freeStrArr(words, CFG_MAX);
}

void simParser(struct Sim *s, char *path)
{
	int wordNum = 0;
	char **words = wordsFromFile(path, CFG_MAX, &wordNum);

	for (int i = 0; i < wordNum; ++i) {
		if (i + 1 < wordNum) {
			if (strcmp(words[i], "end") == 0) {
				break;
			}
			if (strcmp(words[i], "render") == 0) {
				s->rendered = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "simulating") == 0) {
				s->sys.simulating = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "endt") == 0) {
				s->sys.endt = atof(words[i + 1]);
			}
			if (strcmp(words[i], "autoquit") == 0) {
				s->autoquit = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "divhalt") == 0) {
				s->sys.divhalt = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "printitn") == 0) {
				s->sys.printitn = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "epsilon") == 0) {
				s->sys.epsilon = atof(words[i + 1]);
			}
			if (strcmp(words[i], "residual") == 0) {
				s->sys.residual = atof(words[i + 1]);
			}
			if (strcmp(words[i], "bgcol") == 0) {
				s->bgcol = Vec3(atof(words[i + 1]), atof(words[i + 2]), atof(words[i + 3]));
			}
			if (strcmp(words[i], "rz") == 0) {
				s->rz = atof(words[i + 1]);
			}
			if (strcmp(words[i], "rs") == 0) {
				s->rs = atof(words[i + 1]);
			}
			if (strcmp(words[i], "vs") == 0) {
				s->vs = atof(words[i + 1]);
			}
			if (strcmp(words[i], "ps") == 0) {
				s->ps = atof(words[i + 1]);
			}
			if (strcmp(words[i], "rmode") == 0) {
				s->rmode = atof(words[i + 1]);
			}
			if (strcmp(words[i], "relaxm") == 0) {
				s->sys.relaxm = atof(words[i + 1]);
			}
			if (strcmp(words[i], "dtmaxit") == 0) {
				s->sys.dtMaxIt = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "maxit") == 0) {
				s->sys.maxIt = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "gradit") == 0) {
				s->sys.gradIt = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "outputting") == 0) {
				s->outputting = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "outputt") == 0) {
				s->outputt = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "outputf") == 0) {
				s->outputf = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "inputf") == 0) {
				s->inputf = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "inputram") == 0) {
				s->inputram = atoi(words[i + 1]);
			}
			if (strcmp(words[i], "fluid") == 0) {
				s->fluidPath = calloc(strlen(words[i + 1]) + 1, sizeof(char));
				strncpy(s->fluidPath, words[i + 1], strlen(words[i + 1]));
			}
			if (strcmp(words[i], "file") == 0) {
				s->filePath = calloc(strlen(words[i + 1]) + 1, sizeof(char));
				strncpy(s->filePath, words[i + 1], strlen(words[i + 1]));
			}
		}
	}
	
	freeStrArr(words, CFG_MAX);
}

#endif
