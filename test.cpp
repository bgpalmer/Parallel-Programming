#ifndef _OPENMP
#define TEST 0
#else	
#define TEST 1
#endif

#define THREADS	9
#define LIM 20
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;


int main() {
	omp_set_num_threads(THREADS);
	cout << omp_get_num_procs() << endl;

	return 0;
}
