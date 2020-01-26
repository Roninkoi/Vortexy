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
	
	mat t0 = matRandom(10.0f, 3, 4);

	mat t1 = matRandom(10.0f, 3, 1);
	mat t2;

	t0.m[0][0] = 1.0f;
	t0.m[0][1] = 3.0f;
	t0.m[0][2] = 1.0f;
	t0.m[0][3] = 9.0f;
	
	t0.m[1][0] = 1.0f;
	t0.m[1][1] = 1.0f;
	t0.m[1][2] = -1.0f;
	t0.m[1][3] = 1.0f;
	
	t0.m[2][0] = 3.0f;
	t0.m[2][1] = 11.0f;
	t0.m[2][2] = 5.0f;
	t0.m[2][3] = 35.0f;

	matPrint(&t0);

	printf("\n");
	
	GaussElim(&t0);
	
	t2 = GaussSeidel(&t0, &t1);
	
	matPrint(&t0);

	printf("\n");
	
	matPrint(&t1);

	printf("\n");

	matPrint(&t2);

	mat t3 = matMul(&t0, &t2);
	
	printf("\n");

	matPrint(&t3);

	return 0;

	struct Sim sim;

	// initialize simulation
	s_init(&sim);

	// load simulation configuration
	simParser(&sim, cfg);

	// start
	s_run(&sim);

	return 0;
}
