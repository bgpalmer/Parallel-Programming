#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>

using std::setprecision;
using std::cout;
using std::endl;
using std::ofstream;

#define TRIES 100

float A[SZ];
float B[SZ];
float C[SZ];

int main() {
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here!\n");
	return 1;
#endif
	ofstream file;
	file.open("results.txt", std::ios::app);
	
	omp_set_num_threads(NUMT);
	double maxMegaMults = 0.0;
	double sumMegaMults = 0.0;
	for (int i = 0; i < TRIES; i++) {
		double time0 = omp_get_wtime();
		#pragma omp parallel for
		for (int j = 0; j < SZ; j++) {
			C[j] = A[j] * B[j];
		}
		double time1 = omp_get_wtime();
		double megaMults = (double)SZ/(time1-time0) * (1/1000000.00);
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults)
			maxMegaMults = megaMults;
	}
	double avgMegaMults = sumMegaMults/(double)TRIES;
	cout << "NUMT = " << NUMT << ", SZ = " << SZ << "\t";
	cout << "Peak Performance = " << setprecision(8.2) << maxMegaMults << " MegaMults/Sec\t";
	cout << "Average Performance = " << setprecision(8.2) << avgMegaMults << " MegaMults/Sec" << endl;
	if (SZ == 1000) file << NUMT << "\t";
	//file << setprecision(8.2) << maxMegaMults << "\t";
	file << setprecision(8.2) << avgMegaMults;
	if (SZ == 512000) file << "\n";
	else file << "\t";
	file.close();
	return 0;
}
