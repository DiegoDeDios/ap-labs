#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include "logger.h"

void main()
{
	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		infof(" Hola(Thread id: %d) ", ID);
		infof(" Mundo!( Thread id: %d) \n", ID);
	}
}