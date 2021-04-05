#include "sim.h"
#include "util/cfgParser.h"

#include "util/algebra.h"

int main(int argc, char *argv[])
{
	char *cfg;
	char *m;
	int mode = 0; // simulator mode: 0 = simulate, 1 = render

	if (argc < 3) {
		printf("No config file!\n");
		printf("Usage: ./Vortexy <-s / -r> <sim.cfg>\n");

		cfg = "sim.cfg"; // default
	} else {
		m = argv[1];

		cfg = argv[2];

		if (strcmp(m, "-s") == 0) { // simulation mode
			mode = 0;
		}

		if (strcmp(m, "-r") == 0) { // rendering mode
			mode = 1;
		}
	}

	char *date = currentDateTime();

	printf("Vortexy Roninkoi (roninkoi@iki.fi) %s\n", date);

	free(date);

	struct Sim sim;

	sim.mode = mode;

	// initialize simulation
	s_init(&sim);

	// load simulation configuration
	simParser(&sim, cfg);

	// start
	s_run(&sim);

	printf("Closing\n");

	return 0;
}
