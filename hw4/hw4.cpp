#include <omp.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>

int	NowYear = 2017;		// 2017 - 2022
int	NowMonth = 0;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight = 1;		// grain height in inches
int	NowNumDeer = 1;		// number of deer in the current population

float	DiseasePerc;		// MY AGENT

float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX
        float out = (low + r * (high - low ) / (float)RAND_MAX );
	return out;
	//return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;
	int out = (int) Ranf(seedp, low, high);
	return out;
	//return (int)(  Ranf(seedp, low,high) );
}

float SQR( float x )
{
	return x*x;
}



const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				50.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

const float IMMUNITY =				50.0;

FILE *fp = fopen( "agentResults.txt", "w+");

void Grain( void ) {

	while( NowYear < 2023 ) {
		// compute temporary variable based on current global values
		float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
		float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
		float NewHeight = tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		#pragma omp barrier
		#pragma omp atomic
		NowHeight += NewHeight;
		#pragma omp barrier
		#pragma omp barrier
	}	
}

void GrainDeer( void ) {
	while( NowYear < 2023 ) {
		// compute temporary variable based on current global values
		float NewHeight = (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;		
		#pragma omp barrier
		#pragma omp atomic
		NowHeight -= NewHeight;
		#pragma omp barrier
		#pragma omp barrier
	}
}


void Watcher( void ) {
	while( NowYear < 2023 ) {
		// compute temporary var
		#pragma omp barrier
		#pragma omp barrier
		// Print current state	
		printf("NowYear: %d, NowMonth: %d\nNowDeer = %d, NowHeight = %f, NowPrecip = %f, NowTemp = %f, DiseasePerc = %f\n\n", NowYear, (NowMonth % 12) + 1, NowNumDeer, NowHeight, NowPrecip, NowTemp, DiseasePerc); 
		fprintf(fp, "%d\t%d\t%f\t%f\t%f\t%f\n",  1 + NowMonth, NowNumDeer, NowHeight, NowPrecip, NowTemp, DiseasePerc);
	
		// calculate new temperature and precipation values
		if( ++NowMonth % 12 == 0 ) NowYear++;
		float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

		float temp = AVG_TEMP - AMP_TEMP * cos( ang );
		unsigned int seed = 0;
		NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
		NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
		if( NowPrecip < 0. )
			NowPrecip = 0.;
		
		// Decrease or Increase # of deer
		if (NowNumDeer > NowHeight) NowNumDeer--;
		else NowNumDeer++;

		// My Agent
		if ( DiseasePerc > .50 ) { // If more than half of the deer are sick, trigger die-of
			NowNumDeer = NowNumDeer - (NowNumDeer * DiseasePerc);
		}
		if (NowNumDeer < 0) NowNumDeer = 0;
		#pragma omp barrier
	}
}

/* Disease */
void MyAgent( void ) {
	while (NowYear < 2023 ) {
		unsigned int seed = 0;
		float dis_change = ( (float)NowNumDeer / ( IMMUNITY * NowPrecip / NowTemp)); 
		#pragma omp barrier
		DiseasePerc = dis_change;
		#pragma omp barrier
		#pragma omp barrier
	}	
}


int main ( void ) {

	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	unsigned int seed = 0;
	NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
		NowPrecip = 0.;

	omp_set_num_threads( 4 );	// same as # of sections
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer( );
		}

		#pragma omp section
		{
			Grain( );
		}

		#pragma omp section
		{
			Watcher( );
		}

		#pragma omp section
		{
			MyAgent( );	// your own
		}
	}        // implied barrier -- all functions must return in order
		// to allow any of them to get past here	
	return 0;
	fclose( fp );
}	
