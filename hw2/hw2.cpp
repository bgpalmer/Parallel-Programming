#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <omp.h>
#include <string>
#include <ctime>

#define ARRAYSIZE 32 * 1024 
#define TRIES 15
using std::ofstream;

float
ranf( float low, float high )
{
	float r = (float) rand();		// 0 - RAND_MAX

	return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

float array[ARRAYSIZE];

int main() {

#ifndef	_OPENMP
	fprintf( stderr, "OpenMP is not supported - Sorrey!\n");
	return 1;
#endif
	srand(time(NULL));
	ofstream file;
	file.open("results", std::ios::app);
	if (!file.is_open()) {
		fprintf(stderr, "Error opening file!");
		exit(1);
	}
	for (int i = 0; i < ARRAYSIZE; i++) {
		array[i] = ranf(-1.0, 1.0);
	}
	double	sumMegaMults = 0,
		avgMegaMults = 0,
		peakMegaMults = 0;
	long int numMults =(long int)ARRAYSIZE * (long int)(ARRAYSIZE + 1) / 2;
	omp_set_num_threads(NUMT);
	for (int t = 0; t < TRIES; t++) {
		double t0 = omp_get_wtime();
		// 'we have increasing numbers of passes'
		#pragma omp parallel for default(none) schedule(SCHEDULE, CHUNKSIZE) shared(array)
		for (int i = 0; i < ARRAYSIZE - 1; i++) {
			double prod = 1;
			for (int j = 0; j < i; j++) {
				prod *= array[j];
			}
		}
		double t1 = omp_get_wtime();
		double megaMults = ((double)numMults / (t1 - t0) * ( 1 / (double)1000000));
		if (megaMults > peakMegaMults) peakMegaMults = megaMults;
		sumMegaMults += megaMults;
	}
	avgMegaMults = (sumMegaMults / (double) TRIES);
	fprintf(stdout, "average performance = %10.2lf, peak performance = %10.2lf\n", avgMegaMults, peakMegaMults);
	fprintf(stdout, "Threads = %2d, ChunkSize = %5d, MegaMults/sec = %10.2lf\n\n", NUMT, CHUNKSIZE, avgMegaMults);
	file << avgMegaMults << "\t";
	file.close();
	
	return 0;
}	
	
