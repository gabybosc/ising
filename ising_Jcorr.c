#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 5
#define B 0
#define J 0.1
#define PASO 1
#define SIZE 10000

int poblar(int *red);
int flipear(int *red, int *magnetizacion, float *energia);
int imprimir(int *red);
int sumar_sj(int *red, int i);
int calculo_energia(int *red, float *energia);
float func_corr(float *x, int k);


//------------MAIN-------------
int main(){
	// Para cada temperatura calculamos los observables para SIZE pasos correlacionados
	FILE *fp;
	int *red, *magnetizacion, j,k,l,m;
	float *energia;
	red = (int*)malloc(N*N*sizeof(int));
	magnetizacion = (int*)calloc(SIZE, sizeof(int));
	energia = malloc(SIZE* sizeof(float));
	srand(time(NULL));

	float *correlacion;
	correlacion = malloc(30* sizeof(float)); // correlacion para k=0:30

	//correlacion sobre 450 tiras de 200 numeros (luego de 10% pasos de termalizacion)
	int n_tira = 200;
	int cant_tiras = (SIZE-(SIZE/10))/n_tira;
	float x_e[n_tira];
	int x_m[n_tira];

	char fn[50];
	sprintf(fn,"ising_Jcorrelacion.txt");
	fp = fopen(fn, "w");
	fprintf(fp, "J = %f\n",J);
	fprintf(fp, "k\tc_k");

	poblar(red);

	calculo_energia(red, energia);

	for (j = 0; j < N*N; j++){
		*magnetizacion += *(red+j);
	} //calculo magnetizacion total del primer estado

	// OJO: en correlacion, funcion flipear cambia para guardar todos los pasos
	flipear(red, magnetizacion, energia);

	//printf("Mag i = %d\nMag f = %d\nEi = %f\nEf = %f\n",*magnetizacion,*(magnetizacion+SIZE-1),*energia,*(energia+SIZE-1));

	for(k=0; k<30; k++){//k loop para correlacion
		*(correlacion+k) = 0;
		for (l=0; l<cant_tiras;l++){//loop para promediar sobre tiras
			for (m=0; m<n_tira; m++){
				x_e[m] = *(energia+(SIZE/10)+l*n_tira+m);
				//tiras de energia luego de SIZE/10 pasos de termalizacion, de long n_tira
			}//cierra el loop en m
			*(correlacion+k) += func_corr(x_e,k); //hacerlo tambien para x_m?
		}//cierra el loop en l
		*(correlacion+k) = *(correlacion+k)/cant_tiras;
	}//end loop k

	for(k=0; k<30; k++){//k loop para imprimir correlacion
		fprintf(fp, "\n%d\t%f",k,*(correlacion+k));
	}//cierra el loop para imprimir

	free(red);
	free(magnetizacion);
	free(energia);
	free(correlacion);
	fclose(fp);
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


int flipear(int *red, int *magnetizacion, float *energia){ //hacerlo de forma secuencial nos puede traer problemas para T bajas
//flipea un s_inicial a un s_final. Si la energía baja, lo acepta. Si aumenta, lo acepta con una proba P.
	int si,i,j,sum_sj; //si = s en el lugar i
	float P, random;
	float delta_E; //delta_E es la variacion de la energia al flipear (delta_E = 2 * J * sum sisj); ej es la energía total (luego de diezmar)
	int delta_mag = 0;


	for (j = 1; j < SIZE; j++){ //este loop recorre magnetizacion/energia
		random = (float)rand()/(float)RAND_MAX;
		i = ((float)rand()/(float)RAND_MAX)* N*N;
		si = *(red+i);
		sum_sj = sumar_sj(red, i);

		delta_E = 2 * J *si * sum_sj;
		P = exp(-delta_E); //proba de aceptar

		if(delta_E < 0){
			*(red+i) = -si;
			delta_mag -= 2*si;
		}
		else if(random < P){
			*(red+i) = -si;
			delta_mag -= 2*si;
		}else{ //revisar esto! Si no flipea, no cambia la energia ni la magnetizacion
			delta_mag = 0;
			delta_E = 0;
		}
		*(energia+j) = *(energia+(j-1)) + delta_E;
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

int calculo_energia(int *red, float *energia){
	float hamiltoniano = 0;
	int i;

	for(i = 0; i < N*N; i++){
		hamiltoniano += *(red+i) * sumar_sj(red, i);
	}
	*energia = - J * hamiltoniano / 2;
	// printf("hamilt = %f\n", *energia);
	return(0);
}

float func_corr(float *x, int k){
	//x es la cadena de markov de longitud SIZE
	int i;
	float media = 0;
	float media_x = 0;
	float media_sqx = 0;
	float numerador,denominador;

	for (i=0; i<(SIZE-k); i++){
		media = media + (*(x+i+k) * *(x+i)); //valor medio de x(i)*x(i+k)
		media_x = media_x + *(x+i); 		// valor medio de x
		media_sqx = media_sqx + pow(*(x+i),2); //valor medio de x^2
	}//cierra el loop en i
	media = media/(SIZE-k);
	media_x = media_x/(SIZE-k);
	media_x = pow(media_x,2);	// OJO! Ahora media_x es (mean(x))**2
	media_sqx = media_sqx/(SIZE-k);

	numerador = media - media_x;
	denominador = media_sqx - media_x;

return(numerador/denominador);
}
