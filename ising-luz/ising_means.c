#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 64

int poblar(int *red);
int flipear(int *red, float *magnetizacion, float *energia, float B, float J, int size, int paso, float temp);
int sumar_sj(int *red, int i);
int energia_interaccion(int *red, float *energia, float J);
float func_corr(float *x, int n, int k);
float mean_val(float *vector, int length, int abs_bool);

//------------MAIN-------------
int main(){
	// Devuelve los valores medios de magnetizacion y energia
	srand(time(NULL));
	clock_t tic = clock();

	int paso = 1; // paso*N*N pasos entre iteraciones guardadas

	// media sobre n_tira con termalizacion para cada temp
	int n_tira = 10000; //corregir
	int termalizacion = 50000/(N*N) + 1; //porque flipear avanza de a N*N pasos

	float J, B;

	int *red,l,contador=0;
	float *energia, *magnetizacion, *mean_mag, *mean_en, *temps, temp;
	red = (int*)malloc(N*N*sizeof(int));
	magnetizacion = malloc(n_tira* sizeof(float));
	energia = malloc(n_tira* sizeof(float));

	float temp_i = 0.1;
	float temp_f = 4.5;
	float temp_paso = 0.05;

	int size_mean = (temp_f - temp_i)/temp_paso;
	//float porcentaje = temp_paso*100/size_mean;

	mean_mag = calloc(size_mean, sizeof(float));
	mean_en = calloc(size_mean, sizeof(float));
	temps = calloc(size_mean, sizeof(float));

	FILE *fp;
	char fn[50];
	sprintf(fn,"means_N128.txt");
	fp = fopen(fn, "w");

	fprintf(fp, "term %d, %d pasos de N*N\n",termalizacion,n_tira);
	fprintf(fp, "temp\tMag\tEn");

	poblar(red);

	B = 0.0;
	J = -1;
	for (temp=temp_i; temp<temp_f; temp+=temp_paso){//loop temperatura
		*magnetizacion = 0;
		for (l = 0; l < N*N; l++){
			*magnetizacion += *(red+l);
		} //calculo magnetizacion total del primer estado

		energia_interaccion(red, energia, J); //energia de interaccion del primer estado
		*energia -= B * *magnetizacion; // le sumo la energia del campo B

		flipear(red, magnetizacion, energia, B, J, n_tira, paso, temp);
		*(mean_mag+contador) = mean_val(magnetizacion+termalizacion, n_tira-termalizacion,1); //valor medio de magnetizacion
		*(mean_en+contador) = mean_val(energia+termalizacion, n_tira-termalizacion,0); //valor medio de energia
		*(temps+contador) = temp;
		contador++;

	}//end loop en temperatura


	for(l=0; l<size_mean; l++){
		fprintf(fp, "\n%f\t%f\t%f",*(temps+l),*(mean_mag+l)/(N*N),*(mean_en+l)/(N*N));
	}

	free(red);
	free(magnetizacion);
	free(energia);
	fclose(fp);
	clock_t toc = clock();
	printf("\nDuracion: %.2f segundos (%.2f minutos)\a \n", (double)(toc - tic) / CLOCKS_PER_SEC,(double)(toc - tic) / (CLOCKS_PER_SEC*60));
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


int flipear(int *red, float *magnetizacion, float *energia, float B, float J, int size, int paso, float temp){
//flipea un s_inicial a un s_final. Si la energía baja, lo acepta. Si aumenta, lo acepta con una proba P
//guarda magnetizacion y energia cada (paso*N*N) pasos
	int si,i,j,l,sum_sj; //si spin en lugar i, sum_sj suma de vecinos
	float P, random; //prob de aceptacion, nro azar para comparar
	float delta_E, ej; //delta_E es la variacion de la energia al flipear (delta_E = 2 * J * sum sisj); ej es la energía total (luego de diezmar)
	int delta_mag = 0;


	for (j = 1; j < size; j++){ //loop j magnetizacion/energia
		ej = 0; //energia del paso j
		for (l = 0; l < paso*N*N; l++){ //loop l itereaciones salteadas
			random = (float)rand()/(float)RAND_MAX;
			i = ((float)rand()/(float)RAND_MAX)* N*N;
			si = *(red+i);
			sum_sj = sumar_sj(red, i);
			delta_E = 2 * si * (B + J * sum_sj);
			P = exp(-delta_E/temp); //proba de aceptar

			if(delta_E < 0){
				*(red+i) = -si;
				delta_mag -= 2*si;
				ej += delta_E;
			}
			else if(random < P){
				*(red+i) = -si;
				delta_mag -= 2*si;
				ej += delta_E;
			}
		}//cierra el loop en l
		*(energia+j) = *(energia+(j-1)) + ej;
		*(magnetizacion+j) = *magnetizacion + delta_mag;
	}//cierra el loop en j
return 0;
}

int energia_interaccion(int *red, float *energia, float J){
	//calculo de energia de interaccion
	float hamiltoniano = 0;
	int i;

	for(i = 0; i < N*N; i++){
		hamiltoniano += *(red+i) * sumar_sj(red, i);
	}
	*energia = - J * hamiltoniano / 2;
	return(0);
}


float mean_val(float *vector, int length, int abs_bool){
	//valor medio de un vector
	int i;
	float mean=0;

	if (abs_bool==0){
		for (i = 0; i < length; i++){
			mean += *(vector+i);
		}
	}else if (abs_bool==1){
		for (i = 0; i < length; i++){
			mean += abs(*(vector+i));
		}
	}
return(mean/length);
}
