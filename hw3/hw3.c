#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define TRIES 15

#if FIX==1
struct s
{
	float value;
	int pad[NUMPAD];
} Array[NUMT];
#else
struct s
{
	float value;
} Array[NUMT];
#endif

int main( int argv, char *argc[] ) {
	#ifndef _OPENMP
		fprintf(stderr, "OpenMP is not supported -- sorrey!\n");
		exit(1);
	#endif

	#ifndef FIX
		fprintf(stderr, "Macro FIX should be set to 1 or 2, depending on the cache solution you want to run!\n");
		exit(1);
	#endif
		
	FILE * fp = fopen( argc[1], "a+" );
	omp_set_num_threads( NUMT );
	
	float sumMegaSums = 0;
	float maxMegaSums = 0;
	const int SomeBigNumber = 100000000;	// keep < 2B
	for (int t = 0; t < TRIES; t++) {
		float t0, t1; 
		if (FIX == 1) {
			t0 = omp_get_wtime( );
			#pragma omp parallel for 
			for( int i = 0; i < NUMT; i++ )
			{
				unsigned int seed = 0;		// automatically private
				for( unsigned int j = 0; j < SomeBigNumber; j++ )
				{
					Array[ i ].value = Array[ i ].value + (float)rand_r( &seed );
				}
			}
			t1 = omp_get_wtime( );
		}
		if (FIX == 2) {
			t0 = omp_get_wtime( );
			#pragma omp parallel for
                        for( int i = 0; i < NUMT; i++ )
                        {
                                unsigned int seed = 0;          // automatically private
                                float temp = Array[i].value;
				for( unsigned int j = 0; j < SomeBigNumber; j++ )
                                {
                                        temp = temp + (float)rand_r( &seed );
                                }
				Array[ i ].value = temp;
                        }
			t1 = omp_get_wtime( );
		}
		float duration = t1 - t0;
		float megaSum = ((float) (SomeBigNumber * NUMT) / duration / 1000000.0);
		if (megaSum > maxMegaSums) maxMegaSums = megaSum;
		sumMegaSums += megaSum;
	}
	fprintf(stdout, "Padding = %d, Threads = %d, Fix = %d, MaxMegaSums = %8.2lf, Average MegaSums = %8.2lf\n", NUMPAD, NUMT, FIX, maxMegaSums, sumMegaSums / TRIES);
	fprintf(fp, "%8.2lf\t", sumMegaSums / (float)TRIES);
	fclose(fp);
	
	return 0;
}
