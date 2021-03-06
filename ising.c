#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 5
#define B 1
#define J 0
#define PASO 1
#define SIZE 2

int poblar(int *red);
int flipear(int *red, int *magnetizacion);
int imprimir(int *red);

//------------MAIN-------------
int main(){
	// Para cada temperatura (o B) calculamos la magnetizacion para SIZE pasos descorrelacionados
	// FILE *fp;
	int *red, *magnetizacion, j;
	red = (int*)malloc(N*N*sizeof(int));
	magnetizacion = (int*)calloc(SIZE, sizeof(int));
	srand(time(NULL));

	poblar(red);
	printf("Red inicial:\n");
	imprimir(red);

	for (j = 0; j < N*N; j++){
		*magnetizacion += *(red+j);
	}

	flipear(red, magnetizacion);
	printf("Red nueva:\n");
	imprimir(red);
	printf("M =");
	for(j = 0; j<SIZE; j++){
		printf(" %d", *(magnetizacion+j));
	}
	printf("\n");



	// ham = -B * (float)s/PASO;
	// mag = (float)s/PASO;
	// printf("%f %f\n",ham, mag);
	// sprintf(fn,"/home/gabybosc/computacional/datos/nombredelarchivo.txt");
	// fp = fopen(fn, "w"); //"a" es append, mientras que "w" sobreescribe
	free(red);
	free(magnetizacion);

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


int flipear(int *red, int *magnetizacion){ //hacerlo de forma secuencial nos puede traer problemas para T bajas
//flipea un s_inicial a un s_final. Si la energía baja, lo acepta. Si aumenta, lo acepta con una proba P.
	int si,i,j,l; //si = s en el lugar i
	float P, random;
	double delta_E;
	int delta_mag = 0;

	for (j = 1; j < SIZE; j++){
		for (l = 0; l < PASO*N*N; l++){
			random = (float)rand()/(float)RAND_MAX;
			i = ((float)rand()/(float)RAND_MAX)* N*N;
			si = *(red+i);
			delta_E = 2 * B *si;
			P = exp(-delta_E); //proba de aceptar

			if(delta_E < 0){
				*(red+i) = -si;
				delta_mag -= 2*si;
			}
			else if(random < P){
				*(red+i) = -si;
				delta_mag -= 2*si;
			}
		}//cierra el loop en l
	*(magnetizacion+j) = *magnetizacion + delta_mag;
	}//cierra el loop en j

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
