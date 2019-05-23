#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXFILENAME 100

int poblar(int *red, int N, double P);
int imprimir(int *red, int N);

//------------MAIN-------------
int main(){
	FILE *fp;

	sprintf(fn,"/home/gabybosc/computacional/datos/nombredelarchivo.txt");
	fp = fopen(fn, "w"); //"a" es append, mientras que "w" sobreescribe

return 0;
}
//

//----------FUNCIONES----------

int poblar(int *red, int N){
//llena la red cuadrada de lado N con prob P
	int i;
	float P, random;
	P = 0.5;
	for (i=0; i<N*N; i++)
		{random = (float)rand()/(float)RAND_MAX; //se podria mejorar este random
		if (random < P)
			*(red+i) = 1;
		else
			*(red+i) = -1;
		}
return 0;
}


int imprimir(int *red, int N){
//para mostrar la red creada
	int i,j;
	for (i=0; i<N; i++)
		{
		for (j=0; j<N; j++)
			printf("%d ", *(red+N*i+j));
		printf("\n");
		}
return 0;
}
