#include "sim.h"
#include <string.h>
#include "util/algebra.h"
#include <signal.h>

int sk; // sim kill

void handler(int sn)
{
	if (sn == SIGINT) {
		printf("SIGINT\n");
		sk = 1;
	}
}

void s_init(struct Sim *s)
{
	s->running = 1;

	s->time = 0;

	s->timeOld = 0;
	s->tps = 0;
	s->ticks = 0;
	s->ticksOld = 0;
	s->st = 0;

	s->rz = -5.0f;
	s->rs = 1.0f;
	s->vs = 1.0f;
	s->ps = 1.0f;

	convergence = 1;
	sk = 0;

	s->inputi = 0;
	s->inputram = 0;

	signal(SIGINT, handler);

	// intialize random
	srand(timeNow());

	// initialize system
	p_sysInit(&s->sys);
}

// main loop
void s_run(struct Sim *s)
{
#if RENDER_ENABLED == 1
	if (s->rendered)
		r_init(&s->renderer, &s->running);

	s->renderer.camPos.z = s->rz;
	s->renderer.rs = s->rs;
	s->renderer.ps = s->ps;
	s->renderer.vs = s->vs;
	s->renderer.vis = s->rmode;

	s->renderer.clrCol = Vec4(s->bgcol.x, s->bgcol.y, s->bgcol.z, 1.0);
#endif

	p_addObj(&s->sys, s->fluidPath, s->mode);

	if (!s->mode) {
		p_sysStart(&s->sys);
	} else {
		s->outputting = 0;

		if (s->inputram) {
			s->inputdat = wordsFromFile(s->filePath, s->inputram, &s->inputram);
		} else {
			s->file = fopen(s->filePath, "r");
		}

		printf("Input: %s\n", s->filePath);
	}

	if (s->outputting) {
		s->file = fopen(s->filePath, "w");

		printf("Output: %s\n", s->filePath);
	}

	s->startTime = timeMillis();

	while (s->running) {
		s->time = timeMillis() - s->startTime;
		int p = s->time - s->timeOld >= 1000;

		if (p) {
			s->st = s->time - s->timeOld;
			s->timeOld = s->time;
			s->tps = s->ticks - s->ticksOld;
			s->ticksOld = s->ticks;

			printf("ticks: %i, st (ms): %i\n", s->tps, s->st);

			for (int i = 0; i < s->sys.objNum; ++i)
				printf("o %i, t (s): %f\n", i, s->sys.objs[i].t);

			if (!convergence) {
				convergence = 1;

				if (s->divhalt)
					s->sys.simulating = 0; // stop

				printf("Solution not converging!\n");
			}
		}

		s_tick(s);

		if (sk) {
			s->running = 0;
		}

		if (s->sys.reset) {
			s->sys.reset = 0;

			if (!s->mode) {
				p_sysRestart(&s->sys, s->fluidPath);
			} else {
				s->inputi = 0;

				if (!s->inputram)
					rewind(s->file);
			}
		}

		if (!s->sys.simulating && s->autoquit && !s->mode)
			break;

#if RENDER_ENABLED == 1
		if (!s->rendered)
			continue;

		if (p) {
			printf("draws: %i, batches: %i\n", s->renderer.draws, s->renderer.batches);
		}

		if (s->tps > 0) {
			s->renderer.delta = 60.0f / (float) s->tps;
		} else {
			s->renderer.delta = 0.001f;
		}

		r_getInput(&s->renderer, &s->sys);

		if (!s->mode) {
			r_draw(&s->renderer, &s->sys);
		} else {
			r_rdraw(&s->renderer, &s->sys);
		}
#endif
	}

	if (s->outputting || s->mode) {
		if (!s->inputram)
			fclose(s->file);
	}

	if (!s->mode) {
		p_sysEnd(&s->sys);
	}
}

void s_output(struct Sim *s)
{
	fprintf(s->file, "s %i\n", s->ticks);

	for (int i = 0; i < s->sys.objNum; ++i) {
		for (int j = 0; j < s->sys.objs[i].faceNum; ++j) {
			struct Face *f = &s->sys.objs[i].faces[j];
			fprintf(s->file, "o %i", i);

			fprintf(s->file, " t %f f %i", s->sys.objs[i].t, j);
			fprintf(s->file, " x %f %f %f", f->centroid.x, f->centroid.y, f->centroid.z);
			fprintf(s->file, " v %f %f %f", f->v.x, f->v.y, f->v.z);
			fprintf(s->file, " p %f", f->p);
			fprintf(s->file, "\n");
		}
	}

	fprintf(s->file, "e\n");
}

void s_input(struct Sim *s)
{
	while (true) {
		char **words;
		char line[512];
		int n = 512;
		int wn;

		char *fr = fgets(line, n, s->file);

		if (!fr)
			return;

		words = wordsFromStr(line, n, &wn);

		if (strcmp(words[0], "e") == 0) {
			freeStrArr(words, n);

			break;
		}

		if (strcmp(words[0], "o") == 0) {
			int oi = atoi(words[1]);
			int fi = atoi(words[5]);

			float t = atof(words[3]);

			float x = atof(words[7]);
			float y = atof(words[8]);
			float z = atof(words[9]);

			float vx = atof(words[11]);
			float vy = atof(words[12]);
			float vz = atof(words[13]);

			float p = atof(words[15]);

			s->sys.objs[oi].t = t;

			s->sys.objs[oi].faces[fi].v.x = vx;
			s->sys.objs[oi].faces[fi].v.y = vy;
			s->sys.objs[oi].faces[fi].v.z = vz;

			s->sys.objs[oi].faces[fi].p = p;

			freeStrArr(words, n);

			continue;
		}

		freeStrArr(words, n);
	}
}

void s_inputr(struct Sim *s)
{
	while (s->inputi < s->inputram) {
		if (strcmp(s->inputdat[s->inputi+0], "e") == 0) {
			s->inputi += 1;
			break;
		}

		if (strcmp(s->inputdat[s->inputi+0], "o") == 0) {
			int oi = atoi(s->inputdat[s->inputi+1]);
			int fi = atoi(s->inputdat[s->inputi+5]);

			float t = atof(s->inputdat[s->inputi+3]);

			float x = atof(s->inputdat[s->inputi+7]);
			float y = atof(s->inputdat[s->inputi+8]);
			float z = atof(s->inputdat[s->inputi+9]);

			float vx = atof(s->inputdat[s->inputi+11]);
			float vy = atof(s->inputdat[s->inputi+12]);
			float vz = atof(s->inputdat[s->inputi+13]);

			float p = atof(s->inputdat[s->inputi+15]);

			s->sys.objs[oi].t = t;

			s->sys.objs[oi].faces[fi].v.x = vx;
			s->sys.objs[oi].faces[fi].v.y = vy;
			s->sys.objs[oi].faces[fi].v.z = vz;

			s->sys.objs[oi].faces[fi].p = p;

			s->inputi += 16;

			continue;
		}

		s->inputi += 1;
	}
}

void s_tick(struct Sim *s)
{
	if (s->sys.simulating && !s->mode) {
		if (s->outputting && s->ticks % s->outputf == 0)
			s_output(s);

		p_sysTick(&s->sys);
	}

	if (s->mode && s->ticks % s->inputf == 0 && s->sys.simulating) {
		if (s->inputram) {
			s_inputr(s);
		}
		else {
			s_input(s);
		}
	}

	++s->ticks;
}
