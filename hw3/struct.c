#include <stdio.h>
#include <stdlib.h>
#define NUMPAD 15 
#define TYPE int 

struct s
{
        TYPE value;
        int pad[NUMPAD];
} Array[4];
struct b
{
        TYPE value;
} ArrayB[4];


int main() {
	printf("%d\n", sizeof(Array));
	printf("%d\n", (sizeof(Array) / 64 ));
	
	printf("%d\n", sizeof(ArrayB));
        printf("%d\n", (sizeof(ArrayB) / 64 ));
	return 0;
}
