#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 5
#define B 1
#define J 0
#define PASO 1
#define SIZE 10

int poblar(int *red);
int flipear(int *red, int *magnetizacion);
int imprimir(int *red);

//------------MAIN-------------
int main(){
	// FILE *fp;
	int *red, *magnetizacion, j;
	red = (int*)malloc(N*N*sizeof(int));
	magnetizacion = (int*)malloc((SIZE)*sizeof(int)); //habria que escribir un calloc
	srand(time(NULL));

	poblar(red);
	imprimir(red);
	*magnetizacion = 0;

	for (j = 0; j < N*N; j++){
		*magnetizacion += *(red+j);
	}
	printf("M0 = %d\n", *magnetizacion);

	flipear(red, magnetizacion);
	printf("nueva red\n");
	imprimir(red);
	printf("mag \n");
	for(j = 0; j< 3; j++){
		printf("%d ", *(magnetizacion+j));
	}
	printf("\n");



	// ham = -B * (float)s/PASO;
	// mag = (float)s/PASO;
	// printf("%f %f\n",ham, mag);
	// sprintf(fn,"/home/gabybosc/computacional/datos/nombredelarchivo.txt");
	// fp = fopen(fn, "w"); //"a" es append, mientras que "w" sobreescribe
	free(red);

return 0;
}
//

//----------FUNCIONES----------

int poblar(int *red){
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


int flipear(int *red, int *magnetizacion){
//flipea un s_inicial a un s_final. Si la energía baja, lo acepta. Si aumenta, lo acepta con una proba P.
	int si,i,j; //si = s en el lugar i
	float P, random;
	double delta_E;
	int delta_mag;

	delta_mag = 0;

	for (i = 1; i < SIZE*10*N*N; i++){
		random = (float)rand()/(float)RAND_MAX;
		si = *(red+i);
		delta_E = 2 * B *si;
		P = exp(-delta_E); //proba de aceptar

		if(delta_E < 0){
			*(red+i) = -si;
			delta_mag -= 2*si;
			printf("delta mag para delta_E<0 %d\n", delta_mag);
		}
		else if(random < P){
			*(red+i) = -si;
			delta_mag -= 2*si;
			printf("si random %d\n", delta_mag);
		}
		// else{
		// 	delta_mag += 0;
		// }

		if(i % (10*N*N) == 0){
			j = i / (10*N*N);
			*(magnetizacion+j) = *(magnetizacion) + delta_mag;
			printf("deltamag %d\n", delta_mag);
		}
	}

return 0;
}

int imprimir(int *red){
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
