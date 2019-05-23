#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 32
#define B 1
#define PASO 1

int poblar(int *red);
int flipear(int *red, int *s);
int imprimir(int *red);

//------------MAIN-------------
int main(){
	// FILE *fp;
	int *red, i, *s, j;
	float ham, mag; //Hamiltoniano, magnetizacion
	red = (int*)malloc(N*N*sizeof(int));
	s = (int*)malloc((N*N+1)*sizeof(int));
	srand(time(NULL));

	poblar(red);
	imprimir(red);
	for(j=0; j<PASO; j++){
		printf("%nueva red\n");
		flipear(red, s);
		imprimir(red);
	}
	// ham = -B * (float)s/PASO;
	// mag = (float)s/PASO;
	// free(red);
	// printf("%f %f\n",ham, mag);
	// sprintf(fn,"/home/gabybosc/computacional/datos/nombredelarchivo.txt");
	// fp = fopen(fn, "w"); //"a" es append, mientras que "w" sobreescribe

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


int flipear(int *red, int *s){
//flipea un s_inicial a un s_final. Si la energía baja, lo acepta. Si aumenta, lo acepta con una proba P.
	int si,i; //si = s en el lugar i
	float P, random;
	double delta_E;

	for (i = 0; i < N*N; i++){
		*s += *(red+i);
	}

	for (i = 0; i < N*N; i++){
		random = (float)rand()/(float)RAND_MAX;
		si = *(red+i);
		delta_E = 2 * B *si;
		P = exp(-delta_E); //proba de aceptar

		if(delta_E < 0){
			*(red+i) = -si;
			*(s+i+1) = *(s+i) - 2*si;
			// printf("%d\n", si);
		}
		else if(random < P){
			*(red+i) = -si;
			*(s+i+1) = *(s+i) - 2*si;
		}
		else{
			*(s+i+1) = *(s+i);
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
