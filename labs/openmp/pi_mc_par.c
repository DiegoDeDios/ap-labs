#include <stdio.h>
#include <omp.h>
#include "logger.h"
#include "random.h"


//Montecarlo in parallel
int main (char argc, char** argv)
{
    long num_trials = atoi(argv[1]);
	long i; long Ncirc = 0; double pi, x, y;
	double r = 1.0;
	seed(-r, r);
	#pragma omp parallel for private (x, y) reduction (+:Ncirc)
	for(i=0;i<num_trials; i++)
	{
		x = random(); y = random();
		if (( x*x + y*y) <= r*r)
			Ncirc++;
	}
	pi = 4.0 * ((double)Ncirc/(double)num_trials);
	infof("\n %d iterations, pi approximated to: %f \n",num_trials, pi);
}
