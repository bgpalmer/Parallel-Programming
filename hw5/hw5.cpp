#include <omp.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "simd.p5.hpp"

#define TRIES 15

float A[ ARRAYSIZE ];
float B[ ARRAYSIZE ];
float C[ ARRAYSIZE ];


int main ( int argv, char * argc[] ) {

	std::ofstream ofs(argc[1], std::ios::app);
	if (! ofs.is_open()) {
		fprintf(stderr, "file error!\n");
		exit(1);
	}
	double maxMegaMults = 0;
	double sumMegaMults = 0;	
	for (int t = 0; t < TRIES; t++) {
		double t0 = omp_get_wtime();
		for (int i = 0; i < ARRAYSIZE; i++) {
			C[i] = A[i] * B[i];
		}
		double t1 = omp_get_wtime();
		double duration = t1 - t0;
		double megaMults = (double)ARRAYSIZE / duration / 1000000.;
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults)
			maxMegaMults = megaMults;
	}

	// NON-SIMD NON-REDUCTION PRINTOUT 
	printf("ARRAYSIZE = %d, Average MegaMults = %lf, Max MegaMults = %lf\n", ARRAYSIZE, sumMegaMults / TRIES, maxMegaMults);
	ofs << maxMegaMults << "\t";

	maxMegaMults = 0;
	sumMegaMults = 0;
	
	for (int t = 0; t < TRIES; t++) {
                double t0 = omp_get_wtime();
		for (int i = 0; i < ARRAYSIZE; i++) {
			C[i] += A[i] * B[i];
		}	
		double t1 = omp_get_wtime();
                double duration = t1 - t0;
                double megaMults = (double)ARRAYSIZE / duration / 1000000.;
                sumMegaMults += megaMults;
                if (megaMults > maxMegaMults)
                        maxMegaMults = megaMults;
        }
	/* NON-SIMD REDUCTION PRINTOUT */
	printf("ARRAYSIZE = %d, Average MegaMults = %lf, Max MegaMults = %lf\n", ARRAYSIZE, sumMegaMults / TRIES, maxMegaMults);
        ofs << maxMegaMults << "\t";

	/* SIMD-NONREDUCTION */

	maxMegaMults = 0;
        sumMegaMults = 0;

        for (int t = 0; t < TRIES; t++) {
                double t0 = omp_get_wtime();
                SimdMul(A,B,C,ARRAYSIZE);
		double t1 = omp_get_wtime();
                double duration = t1 - t0;
                double megaMults = (double)ARRAYSIZE / duration / 1000000.;
                sumMegaMults += megaMults;
                if (megaMults > maxMegaMults)
                        maxMegaMults = megaMults;
        }
        /* SIMD Non-REDUCTION PRINTOUT */
        printf("ARRAYSIZE = %d, Average MegaMults = %lf, Max MegaMults = %lf\n", ARRAYSIZE, sumMegaMults / TRIES, maxMegaMults);
        ofs << maxMegaMults << "\t";	
	
	/* SIMD-REDUCTION */
	maxMegaMults = 0;
        sumMegaMults = 0;

        for (int t = 0; t < TRIES; t++) {
                double t0 = omp_get_wtime();
                SimdMulSum(A,B,ARRAYSIZE);
		double t1 = omp_get_wtime();
                double duration = t1 - t0;
                double megaMults = (double)ARRAYSIZE / duration / 1000000.;
                sumMegaMults += megaMults;
                if (megaMults > maxMegaMults)
                        maxMegaMults = megaMults;
        }
        /* SIMD REDUCTION PRINTOUT */
        printf("ARRAYSIZE = %d, Average MegaMults = %lf, Max MegaMults = %lf\n", ARRAYSIZE, sumMegaMults / TRIES, maxMegaMults);
        ofs << maxMegaMults << "\t";

	ofs.close();	

	return 0;
} 
