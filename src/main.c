#include "sim.h"
#include "util/cfgParser.h"

#include "util/algebra.h"

int main(int argc, char *argv[])
{
	char *cfg;

	if (argc < 2) {
		printf("No config file!\n");
		printf("Usage: ./Vortexy <sim.cfg>\n");

		cfg = "sim.cfg"; // default
	}
	else {
		cfg = argv[1];
	}

	char *date = currentDateTime();

	printf("Vortexy Roninkoi %s\n", date);

	free(date);

	struct Sim sim;

	// initialize simulation
	s_init(&sim);

	// load simulation configuration
	simParser(&sim, cfg);

	// start
	s_run(&sim);

	return 0;
}
