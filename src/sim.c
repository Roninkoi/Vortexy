#include "sim.h"

void s_init(struct Sim *s)
{
	s->running = 1;

	s->time = 0.0f;
	s->dt = 1.0f;

	s->timeOld = timeNow();
	s->tps = 60;
	s->ticks = 0;
	s->ticksOld = 0;

	// intialize random
	srand(timeNow());

	// initialize system
	p_sysInit(&s->sys);

#if OPENCL_ENABLED == 0
	s->usegpu = 0;
#endif
}

// main loop
void s_run(struct Sim *s)
{
#if RENDER_ENABLED == 1
	if (s->rendered)
		r_init(&s->renderer, &s->running);
#endif
	
	p_addObj(&s->sys, s->meshPath, s->fluidPath);

	p_sysStart(&s->sys);

	s->startTime = timeNow();

	while (s->running) {
		s->time = timeNow() - s->startTime;
		int p = s->time - s->timeOld >= 1;
	  
		if (p) {
			s->timeOld = s->time;
			s->tps = s->ticks - s->ticksOld;
			s->ticksOld = s->ticks;

			printf("time [s]: %.2f, ticks/s: %i\n", s->t, s->tps);
		}

		s_tick(s);

#if RENDER_ENABLED == 1
		if (!s->rendered)
			continue;
		
		if (p) {
			printf("draws: %i, batches: %i\n", s->renderer.draws, s->renderer.batches);
		}

		if (s->tps > 0) {
			s->renderer.delta = 60.0f / (float) s->tps;
		}
		else {
			s->renderer.delta = 0.001f;
		}

		r_getInput(&s->renderer, &s->sys);

		r_draw(&s->renderer, &s->sys);
#endif
	}
}

void s_tick(struct Sim *s)
{
	++s->ticks;

	s->t += (double) s->dt; // increment time
	s->sys.dt = s->dt;

	s->sys.time = (float) timeMillis() / 1000.0f;

	p_sysTick(&s->sys);
}
