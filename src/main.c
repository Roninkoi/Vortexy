#include "sim.h"
#include "util/cfgParser.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("No config file!\n");
		printf("Usage: ./Vortexy <sim.cfg>\n");
		return 0;
	}

	char *date = currentDateTime();

	printf("Vortexy Roninkoi %s\n", date);

	free(date);

	struct Sim sim;

	// initialize simulation
	s_init(&sim);

	// load simulation configuration
	simParser(&sim, argv[1]);

	// start
	s_run(&sim);

	return 0;
}
