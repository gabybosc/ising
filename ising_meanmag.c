#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define B 1.0 	// campo magnetico
#define N 32 	// lado de la red
#define J 0 	// interaccion entre particulas
#define PASO 10 	// descorrelacion: PASO*SIZE*N^2
<<<<<<< HEAD
#define SIZE 100000 	// tamaño de vectores diezmados
=======
#define SIZE 1000000 	// tamaño de vectores diezmados
>>>>>>> 66649ed275006f3bafbe2e76921f4720443fba0a
#define TERM 20000	// pasos de termalizacion

int poblar(int *red);
int flipear(int *red, int *magnetizacion, float beta);
int imprimir(int *red);

//------------MAIN-------------
int main(){
	// Para cada temperatura (o B) calculamos la magnetizacion para SIZE pasos descorrelacionados
	clock_t tic = clock();
	FILE *fp;
	char fn[30];
	int *red, *magnetizacion, j, j_min;
	float mean_mag, beta, energia_sitio;
	red = (int*)malloc(N*N*sizeof(int));
	magnetizacion = (int*)calloc(SIZE, sizeof(int));
	srand(time(NULL));
	j_min = TERM/(PASO*N*N);

	sprintf(fn,"magnetizacion_vs_temp.txt");
	fp = fopen(fn, "w"); //"a" es append, mientras que "w" sobreescribe
	fprintf(fp,"Delta_iteraciones=%d**2*%d\tJ=%d\n",N,PASO,J);
	fprintf(fp,"beta\t\t\t\t\tm\t\t\t\t\tu");

	for (beta = 0.2; beta <5; beta+=0.2){ //loop en temperaturas
		poblar(red);
		*magnetizacion = 0;
		for (j = 0; j < N*N; j++){
			*magnetizacion += *(red+j);
		}//cierra el loop en j

		flipear(red, magnetizacion, beta);

		mean_mag = 0;
		for(j = j_min; j<SIZE; j++){
			mean_mag += *(magnetizacion+j);
		} //cierra el loop en j
		mean_mag = mean_mag/((SIZE-j_min)*N*N);
		energia_sitio = -B * mean_mag;
		fprintf(fp,"\n%f\t%f\t%f",beta,mean_mag, energia_sitio);
	} // cierra el loop en beta

	free(red);
	free(magnetizacion);

	clock_t toc = clock();
	printf("\a Duracion: %f segundos (%f minutos)\n", (double)(toc - tic) / CLOCKS_PER_SEC,(double)(toc - tic) / (CLOCKS_PER_SEC*60));

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


int flipear(int *red, int *magnetizacion, float beta){
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
			delta_E = 2 * beta * B *si;
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
