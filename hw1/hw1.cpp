#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <iomanip>
#include <fstream>

using std::ofstream;
using std::setprecision;
using std::cout;
using std::endl;

#define XMIN	0.
#define XMAX	3.
#define YMIN	0.
#define YMAX	3.

#define TOPZ00  0.
#define TOPZ10  1.
#define TOPZ20  0.
#define TOPZ30  0.

#define TOPZ01  1.
#define TOPZ11  6.
#define TOPZ21  1.
#define TOPZ31  0.

#define TOPZ02  0.
#define TOPZ12  1.
#define TOPZ22  0.
#define TOPZ32  4.

#define TOPZ03  3.
#define TOPZ13  2.
#define TOPZ23  3.
#define TOPZ33  3.

#define BOTZ00  0.
#define BOTZ10  -3.
#define BOTZ20  0.
#define BOTZ30  0.

#define BOTZ01  -2.
#define BOTZ11  10.
#define BOTZ21  -2.
#define BOTZ31  0.

#define BOTZ02  0.
#define BOTZ12  -5.
#define BOTZ22  0.
#define BOTZ32  -6.

#define BOTZ03  -3.
#define BOTZ13   2.
#define BOTZ23  -8.
#define BOTZ33  -3.

// Number of times to calulate Volume with specified NUMNODE and NUMT definitions
#define TRIES 20

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float u = (float)iu / (float)(NUMNODES-1);
	float v = (float)iv / (float)(NUMNODES-1);

	// the basis functions:

	float bu0 = (1.-u) * (1.-u) * (1.-u);
	float bu1 = 3. * u * (1.-u) * (1.-u);
	float bu2 = 3. * u * u * (1.-u);
	float bu3 = u * u * u;

	float bv0 = (1.-v) * (1.-v) * (1.-v);
	float bv1 = 3. * v * (1.-v) * (1.-v);
	float bv2 = 3. * v * v * (1.-v);
	float bv3 = v * v * v;

	float top =       bu0 * ( bv0*TOPZ00 + bv1*TOPZ01 + bv2*TOPZ02 + bv3*TOPZ03 )
                        + bu1 * ( bv0*TOPZ10 + bv1*TOPZ11 + bv2*TOPZ12 + bv3*TOPZ13 )
                        + bu2 * ( bv0*TOPZ20 + bv1*TOPZ21 + bv2*TOPZ22 + bv3*TOPZ23 )
                        + bu3 * ( bv0*TOPZ30 + bv1*TOPZ31 + bv2*TOPZ32 + bv3*TOPZ33 );

        float bot =       bu0 * ( bv0*BOTZ00 + bv1*BOTZ01 + bv2*BOTZ02 + bv3*BOTZ03 )
                        + bu1 * ( bv0*BOTZ10 + bv1*BOTZ11 + bv2*BOTZ12 + bv3*BOTZ13 )
                        + bu2 * ( bv0*BOTZ20 + bv1*BOTZ21 + bv2*BOTZ22 + bv3*BOTZ23 )
                        + bu3 * ( bv0*BOTZ30 + bv1*BOTZ31 + bv2*BOTZ32 + bv3*BOTZ33 );

	return top - bot; 
};

int main() {
#ifndef _OPENMP
        fprintf(stderr, "OpenMP is not supported -- sorrey!\n");
        exit(0);
#endif	
	omp_set_num_threads(NUMT);	
	double 	maxMegaHeights = 0.0, 
		avgMegaHeights = 0.0,
		sumMegaHeights = 0.0;

	// the area of a single full-sized tile"
	float fullTileArea = (((XMAX-XMIN) / (float)(NUMNODES - 1)) * ((YMAX - YMIN) / (float)(NUMNODES - 1)));
	
	for (int t = 0; t < TRIES; t++) {

		// weighted heights are summed into the variable "volume" using an OpenMP for lopp and a reduction:
		double sum = 0.0;
		double t0 = omp_get_wtime();
		#pragma omp parallel for default(none) shared(fullTileArea) reduction(+:sum)
		for (int i = 0; i < NUMNODES * NUMNODES; i++) {
			int iu = i % NUMNODES;
			int iv = i / NUMNODES;
			if ((iv == 0 && iu == 0) || (iv == 0 && iu == NUMNODES - 1) || (iv == NUMNODES - 1 && iu == NUMNODES - 1) || (iv == NUMNODES - 1 && iu == 0))
				sum += (fullTileArea / 4) * Height(iu,iv); 
			else if (iv == 0 || iu == 0 || iv == NUMNODES - 1 || iu == NUMNODES - 1) 
				sum += (fullTileArea / 2) * Height(iu,iv);
			else 
				sum += fullTileArea * Height(iu,iv);
		}
		double t1 = omp_get_wtime();
		double megaHeights = ((double) (NUMNODES * NUMNODES) / (t1 - t0)) * (1/1000000.00); 
		sumMegaHeights += megaHeights;
		if (megaHeights > maxMegaHeights) maxMegaHeights = megaHeights;
	}
	avgMegaHeights = sumMegaHeights / (double) TRIES;
	
	// cout << setprecision(8.2) << "MAX = " << maxMegaHeights << ", AVG = " << avgMegaHeights << endl;
	ofstream avgFile, peakFile;
	avgFile.open("avgPerf.txt", std::ios::app);
	peakFile.open("peakPerf.txt", std::ios::app);
	if (NUMNODES == 2) {
		avgFile << NUMT << "\t";
		peakFile << NUMT << "\t";
	}
	avgFile << setprecision(8.2) << avgMegaHeights;
	peakFile << setprecision(8.2) << maxMegaHeights;
	if (NUMNODES == 32768) {
		avgFile << "\n";
		peakFile << "\n";
	}
	else {
		avgFile << "\t";
		peakFile << "\t";
	}
	return 0;
}
	
		
	
