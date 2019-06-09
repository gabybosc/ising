#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 32
#define B 0.0
//#define J 0.01
#define SIZE 400000000

int poblar(int *red);
int flipear(int *red, int *magnetizacion, float J);
int imprimir(int *red);
int sumar_sj(int *red, int i);
//int energia_interaccion(int *red, float *energia, float J);
float func_corr(float *x, int n, int k);


//------------MAIN-------------
int main(){ //(int argc,char *argv[])
	// Para cada temperatura calculamos los observables para SIZE pasos correlacionados
	// Guarda un txt que devuelve la correlacion ya promediada 
	srand(time(NULL));
	clock_t tic = clock();
	
	FILE *fp;
	int *red, *magnetizacion, i,l,k;
	//float *energia;
	float *correlacion;
	red = (int*)malloc(N*N*sizeof(int));
	magnetizacion = (int*)calloc(SIZE, sizeof(int));
	//energia = malloc(SIZE* sizeof(float));
	
	int k_max = 300; //NOTA: Esto no es k, es k per site: (k/(N*N))
	correlacion = malloc(k_max* sizeof(float));
	
	//correlacion sobre tiras de longitud n_tira (luego de pasos de termalizacion)
	int n_tira = 4000000;
	int termalizacion = 20000;
	int cant_tiras = (SIZE-termalizacion)/n_tira;
	float *x;
	x = malloc(n_tira * sizeof(float));
	
	float J;
	
	char fn[50];
	sprintf(fn,"ising_correlacion_Jcrit.txt");
	fp = fopen(fn, "w");
	fprintf(fp, "B %f; J [0.4; 0.65]; cadena %d, term %d, %d tiras de %d pasos, kps_max %d\n",B,SIZE,termalizacion,cant_tiras,n_tira,k_max);
	fprintf(fp, "Para cada J, C(k) para magnetizacion\n");

	for (J=0.4; J<0.65; J+=0.05){//loop J
		poblar(red);
		
		for (l = 0; l < N*N; l++){
			*magnetizacion=0;
		} //set magnetizacion en 0
		
		for (l = 0; l < N*N; l++){
			*magnetizacion += *(red+l);
		} //calculo magnetizacion total del primer estado
		
		//energia_interaccion(red, energia, J); //energia de interaccion del primer estado
		//*energia = *energia - B * *(magnetizacion); // le sumo la energia del campo B

		// OJO: en correlacion, funcion flipear cambia para guardar todos los pasos
		flipear(red, magnetizacion, J);
		
		
		for (k=0; k<k_max; k++){
			*(correlacion+k) = 0;
		}//set correlacion a 0
		for (i=0; i<(cant_tiras-1); i++){
			for (l=0; l<n_tira; l++){ //armo la tira corta
				*(x+l) = *(magnetizacion + termalizacion + i*n_tira + l);
			}
			for (k=0; k<k_max; k++){ //para cada tira corta, calculo la correlacion para todos los k
				*(correlacion+k) += func_corr(x,n_tira,k*N*N); //calculo la correlacion moviendome de a N*N sitios
			}
		}//loop de sobre las tiras
		for (k=0; k<k_max; k++){
			*(correlacion+k) = *(correlacion+k)/(cant_tiras-1);
		}//hago el promedio
		for(k=0; k<k_max; k++){
			fprintf(fp, "%f ",*(correlacion+k));
		}//k loop para imprimir correlacion
		fprintf(fp, "\n");
	}//end loop J
	
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


int flipear(int *red, int *magnetizacion, float J){ //hacerlo de forma secuencial nos puede traer problemas para T bajas
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

		delta_E = 2 * si * (J * sum_sj + B);
		P = exp(-delta_E); //proba de aceptar

		if(delta_E < 0){
			*(red+i) = -si;
			delta_mag -= 2*si;
		}
		else if(random < P){
			*(red+i) = -si;
			delta_mag -= 2*si;
		}else{ //revisar esto! Si no flipea, no cambia la energia ni la magnetizacion
			//delta_mag = 0;
			delta_E = 0;
		}
		//*(energia+j) = *(energia+(j-1)) + delta_E;
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
/*
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
*/

float func_corr(float *x, int n, int k){
	//x es la cadena de markov de longitud n
	int i;
	float media = 0;
	float media_x = 0;
	float media_sqx = 0;
	float numerador,denominador;
	
	for (i=0; i<(n-k); i++){
		media += *(x+i+k) * *(x+i); //valor medio de x(i)*x(i+k)
		media_x += *(x+i); 		// valor medio de x
		media_sqx += pow(*(x+i),2); //valor medio de x^2
	}
	media = media/(n-k);
	media_x = media_x/(n-k);
	media_x = pow(media_x,2);	// OJO! Ahora media_x es (mean(x))**2
	media_sqx = media_sqx/(n-k);
	
	numerador = media - media_x;
	denominador = media_sqx - media_x;

return(numerador/denominador);
}
