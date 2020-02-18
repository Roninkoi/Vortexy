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
	
	mat t0 = matRandom(10.0f, 5, 5);

	mat t1 = matRandom(10.0f, 5, 1);
	mat t2;

	t0.m[0][0] = 6.0f;
	t0.m[0][1] = 1.0f;
	t0.m[0][2] = 0.0f;
	t0.m[0][3] = 3.0f;
	t0.m[0][4] = 3.0f;
	
	t0.m[1][0] = 0.0f;
	t0.m[1][1] = 6.0f;
	t0.m[1][2] = 0.0f;
	t0.m[1][3] = 4.0f;
	t0.m[1][4] = 3.0f;
	
	t0.m[2][0] = 0.0f;
	t0.m[2][1] = 5.0f;
	t0.m[2][2] = 6.0f;
	t0.m[2][3] = 1.0f;
	t0.m[2][4] = 0.0f;

	t0.m[3][0] = 5.0f;
	t0.m[3][1] = 5.0f;
	t0.m[3][2] = 0.0f;
	t0.m[3][3] = 6.0f;
	t0.m[3][4] = 0.0f;

	t0.m[4][0] = 2.0f;
	t0.m[4][1] = 0.0f;
	t0.m[4][2] = 2.0f;
	t0.m[4][3] = 4.0f;
	t0.m[4][4] = 10.0f;

	t1.m[0][0] = 22.0f;
	t1.m[1][0] = 21.0f;
	t1.m[2][0] = 20.0f;
	t1.m[3][0] = 10.0f;
	t1.m[4][0] = 22.0f;

	matPrint(&t0);

	printf("\n");

	t2 = GaussSeidel(&t0, &t1);

	printf("\n");
	
	matPrint(&t1);

	printf("\n");

	matPrint(&t2);

	mat t3 = matMul(&t0, &t2);
	
	printf("\n");

	//return 0;

	struct Sim sim;

	// load simulation configuration
	simParser(&sim, cfg);
	
	// initialize simulation
	s_init(&sim);

	// start
	s_run(&sim);

	return 0;
}
