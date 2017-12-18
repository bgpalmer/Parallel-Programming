#include <stdlib.h>
#include <stdio.h>
int main () {

	for (int i = 0; i < 5; i++) {
		unsigned int seed = 1;
		printf("%f\t", rand_r( &seed ) );
	}
	printf("\n");
	return 0;
}
