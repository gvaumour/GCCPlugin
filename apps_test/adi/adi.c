/**
*
Alternating direction implicit method
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


#ifndef NUM_THREADS
	#define NUM_THREADS 4 
#endif

#ifdef VERIF
	#define TSTEPS 1
	#define N 32
#else 
	#define TSTEPS 10
	#define N 300
#endif 

#define _PB_TSTEPS TSTEPS
#define _PB_N N 


/* Array initialization. */
static
void init_array (int n, double** X,double** A, double** B)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			X[i][j] = ((double) i*(j+1) + 1) / n;
			A[i][j] = ((double) i*(j+2) + 2) / n;
			B[i][j] = ((double) i*(j+3) + 3) / n;
		}
}


/* DCE code. Must scan the entire live-out data.
Can be used also to check the correctness of the output. */
static
void printf_array(int n,double** X)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			if ((i * N + j) % 20 == 0) fprintf(stderr, "\n");
		}

}


/* Main computational kernel. The whole function will be timed,
including the call and return. */
static
void kernel_adi(int tsteps, int n,double** X,double** A,double** B)
{
	int t;
	register int i1, i2;
	#pragma omp parallel num_threads(NUM_THREADS)
	{
		__builtin_prefetch(A , 0 , 2);
		__builtin_prefetch( &A[n-1][n-1] , 0 , 1);	
	}
	
//	#pragma scop
	#pragma omp parallel for private(i1, t,i2) num_threads(NUM_THREADS) schedule(static)
	for (t = 0; t < _PB_TSTEPS; t++)
	{	for (i1 = 0; i1 < _PB_N; i1++)
			for (i2 = 1; i2 < _PB_N; i2++)
			{
				X[i1][i2] = X[i1][i2] - X[i1][i2-1] * A[i1][i2] / B[i1][i2-1];
				B[i1][i2] = B[i1][i2] - A[i1][i2] * A[i1][i2] / B[i1][i2-1];
			}

		for (i1 = 0; i1 < _PB_N; i1++)
			X[i1][_PB_N-1] = X[i1][_PB_N-1] / B[i1][_PB_N-1];

		for (i1 = 0; i1 < _PB_N; i1++)
			for (i2 = 0; i2 < _PB_N-2; i2++)
				X[i1][_PB_N-i2-2] = (X[i1][_PB_N-2-i2] - X[i1][_PB_N-2-i2-1] * A[i1][_PB_N-i2-3]) / B[i1][_PB_N-3-i2];


		for (i1 = 1; i1 < _PB_N; i1++)
			for (i2 = 0; i2 < _PB_N; i2++) {
			  X[i1][i2] = X[i1][i2] - X[i1-1][i2] * A[i1][i2] / B[i1-1][i2];
			  B[i1][i2] = B[i1][i2] - A[i1][i2] * A[i1][i2] / B[i1-1][i2];
			}

		for (i2 = 0; i2 < _PB_N; i2++)
			X[_PB_N-1][i2] = X[_PB_N-1][i2] / B[_PB_N-1][i2];

		for (i1 = 0; i1 < _PB_N-2; i1++)
			for (i2 = 0; i2 < _PB_N; i2++)
			  X[_PB_N-2-i1][i2] = (X[_PB_N-2-i1][i2] - X[_PB_N-i1-3][i2] * A[_PB_N-3-i1][i2]) / B[_PB_N-2-i1][i2];
	}

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		__builtin_prefetch(A , 0 , 2);
		__builtin_prefetch( &A[n-1][n-1] , 0 , 1);	
	}

}


int main(int argc, char** argv)
{
	/* Retrieve problem size. */
	int n = N;
	int tsteps = TSTEPS;

	printf("Tab allocation ...\n");

	double** X = (double** ) malloc(sizeof(double*) * N);
	double** B = (double** ) malloc(sizeof(double*) * N);
	double** A = (double** ) malloc(sizeof(double*) * N);

	int i;

	for( i = 0 ; i < N ; i++){
		X[i] = malloc(sizeof(double) * N);
		B[i] = malloc(sizeof(double) * N);
		A[i] = malloc(sizeof(double) * N);
	}

	/* Initialize array(s). */
	printf("init_array ... \n");
	init_array (n, X, A, B);

	/* Run kernel. */
	printf("Running kernel ... \n");
	kernel_adi (tsteps, n, X, A, B);

	/* Prevent dead-code elimination. All live-out data must be printf
	by the function call in argument. */
	if(argc > 25)
		printf_array(n, X);

	for( i = 0 ; i < N ; i++){
		free(X[i]);
		free(B[i]);
		free(A[i]);
	}
	free(A);
	free(X);
	free(B);

	return 0;
}
