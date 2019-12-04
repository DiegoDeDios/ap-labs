#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include "logger.h"

double riemann(long step_range, int thread_num, double step)
{
	double sum = 0.0;
	double x = 0.0;
	int lowerbound = step_range * thread_num;
	int upperbound = step_range * (thread_num + 1);
	infof("Thread: %d\n step_range: %d\n lower: %d\n upper: %d\n", thread_num, step_range, lowerbound, upperbound);
	for(int i = lowerbound; i < upperbound; i++){
		x = (i+0.5)*step;
		sum = sum + 4.0/(1.0+x*x);
		}
	return sum;
}

//static long num_steps = 100000;
double step;
void main (char argc, char** argv)
{ 
int i;
long num_steps = atoi(argv[1]);
double sumthreads[4];
double x, pi, sum = 0.0;
step = 1.0/(double) num_steps;

#pragma omp parallel
	{
int mytid = omp_get_thread_num();
sumthreads[mytid] = riemann(num_steps/4, mytid, step);
#pragma omp barrier
sum = sumthreads[0] + sumthreads[1] + sumthreads[2] + sumthreads[3];
	}

pi = step * sum;
infof("%f\n", pi);
}


