#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 32
#define B 0.0
#define J 0.1
#define PASO 20 //k / N*N
#define SIZE 10000000

int poblar(int *red);
int flipear(int *red, float *magnetizacion, float *energia);
int imprimir(int *red);
int sumar_sj(int *red, int i);
int energia_interaccion(int *red, float *energia);
float func_corr(float *x, int n, int k);


//------------MAIN-------------
int main(){
	// Para cada temperatura calculamos los observables para SIZE pasos correlacionados
	srand(time(NULL));
	clock_t tic = clock();

	FILE *fp;
	int *red, i,l,k,contador=0;
	float *energia, *magnetizacion, beta;
	//float *correlacion;
	red = (int*)malloc(N*N*sizeof(int));
	magnetizacion = malloc(SIZE* sizeof(float));
	energia = malloc(SIZE* sizeof(float));

	int n_tira = 1000; //corregir

	float *mean_mag,*mean_en;
	mean_mag = calloc(n_tira, sizeof(float));
	mean_en = calloc(n_tira, sizeof(float));

	char fn[50];
	sprintf(fn,"ising_mean.txt");
	fp = fopen(fn, "w");
	fprintf(fp, "B = %f; J en 0.1 - 0.3; cadena %d, term %d, %d tiras de %d pasos, kps_max %d\n",B,SIZE,termalizacion,cant_tiras,n_tira,k_max);
	//fprintf(fp, "Para cada J, C(k) para magnetizacion\n");

	poblar(red);

	for (beta=0.1; beta<5; beta+=0.1){//loop temperatura

		for (l = 0; l < SIZE; l++){
			*(magnetizacion+l)=0;
		} //set magnetizacion en 0
		for (l = 0; l < N*N; l++){
			*magnetizacion += *(red+l);
		} //calculo magnetizacion total del primer estado

		energia_interaccion(red, energia); //energia de interaccion del primer estado
		*energia = *energia - B * *(magnetizacion); // le sumo la energia del campo B

		// OJO: en correlacion, funcion flipear cambia para guardar todos los pasos
		flipear(red, magnetizacion, energia);
		*(mean_mag+contador) = mean_val(); //valor medio de magnetizacion (completo)
		*(mean_en+contador) = mean_val(); //valor medio de energia (completo)
		contador++;
	}//end loop en temperatura

	for(k=0; k<k_max; k++){
		fprintf(fp, "%f ",*(magnetizacion+k));
	}
	fprintf(fp, "\n");

	free(red);
	free(magnetizacion);
	//free(energia);
	free(correlacion);
	free(x);
	fclose(fp);
	clock_t toc = clock();
	printf("Duracion: %f segundos (%f minutos)\n", (double)(toc - tic) / CLOCKS_PER_SEC,(double)(toc - tic) / (CLOCKS_PER_SEC*60));
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

int sumar_sj(int *red, int i){
	int sum_sj = 0;

	// primero vemos las esquinas
	if(i == 0){
		sum_sj = *(red+1) + *(red+N) + *(red+(N*(N-1))) + *(red+(N-1));
	}
	else if(i == N -1){
		sum_sj = *(red+(N-2)) + *red + *(red+(2*N-1)) + *(red+(N*N-1));
	}
	else if(i == N*(N-1)){
		sum_sj = *red + *(red+(N*(N-1)+1)) + *(red+(N*N-1)) + *(red+(N*N-(2*N)));
	}
	else if(i == N*N-1){
		sum_sj = *(red+(N*(N-1))) + *(red+(N*N-N-1)) + *(red+(N*N-2)) + *(red+(N-1));
	}
	//si no es esquina, vemos laterales
	else if(i < N){ //la tira superior
		sum_sj = *(red+(i+1)) + *(red+(i-1)) + *(red+(i+N)) + *(red+(i+N*(N-1)));
	}
	else if(i % N == 0){ //el lado izq
		sum_sj = *(red+(i+1)) + *(red+(i-N)) + *(red+(i+N)) + *(red+(i+N-1));
	}
	else if((i+1) % N == 0){ //el lado derecho
		sum_sj = *(red+(i-1)) + *(red+(i-N)) + *(red+(i+N)) + *(red+(i-N+1));
	}
	else if(i > N*(N-1)){ //la tira de abajo
		sum_sj = *(red+(i-1)) + *(red+(i+1)) + *(red+(i-N)) + *(red+(i-N*(N-1)));
	}
	//si esta en el medio:
	else{
		sum_sj = *(red+(i-1)) + *(red+(i+1)) + *(red+(i-N)) + *(red+(i+N));
	}

return sum_sj;
}


int flipear(int *red, float *magnetizacion, float *energia){ //hacerlo de forma secuencial nos puede traer problemas para T bajas
//flipea un s_inicial a un s_final. Si la energía baja, lo acepta. Si aumenta, lo acepta con una proba P.
	int si,i,j,l, sum_sj; //si = s en el lugar i
	float P, random;
	float delta_E, ej; //delta_E es la variacion de la energia al flipear (delta_E = 2 * J * sum sisj); ej es la energía total (luego de diezmar)
	int delta_mag = 0;


	for (j = 1; j < SIZE; j++){ //este loop recorre magnetizacion/energia
		ej = 0;
		for (l = 0; l < PASO*N*N; l++){ //este loop recorre la red
			random = (float)rand()/(float)RAND_MAX;
			i = ((float)rand()/(float)RAND_MAX)* N*N;
			si = *(red+i);
			sum_sj = sumar_sj(red, i);

			delta_E = 2 * J *si * sum_sj;
			ej += delta_E;
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
	*(energia+j) = *(energia+(j-1)) + ej;
	*(magnetizacion+j) = *magnetizacion + delta_mag;
	}//cierra el loop en j

return 0;
}

int energia_interaccion(int *red, float *energia){
	//calculo de energia de interaccion
	float hamiltoniano = 0;
	int i;

	for(i = 0; i < N*N; i++){
		hamiltoniano += *(red+i) * sumar_sj(red, i);
	}
	*energia = - J * hamiltoniano / 2;
	// printf("hamilt = %f\n", *energia);
	return(0);
}


int mean_val(){


	return 0;
}
