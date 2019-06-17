#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 32

int poblar(int *red);
int imprimir(int *red);
int flipear(int *red, int *magnetizacion, float *energia, float B, float J, int size, int paso);
int sumar_sj(int *red, int i);
int energia_interaccion(int *red, float *energia, float J);
float func_corr(float *x, int n, int k);

//------------MAIN-------------
int main(){
	// devuelve correlacion media
	srand(time(NULL));
	clock_t tic = clock();
	
	int paso = 1; // NO CAMBIAR. paso*N*N pasos entre iteraciones guardadas
	int k_max = 50; // Esto NO es k, es k per site: (k/(N*N))
	
	//correlacion media sobre cant_tiras de longitud n_tira con termalizacion
	int n_tira = 5000;
	int termalizacion = 9000/(N*N) + 1; //porque flipear avanza de a N*N pasos
	int cant_tiras = 100;
	
	float B,J;
	
	int *red, *magnetizacion, m,l,k;
	float *correlacion, *energia, *x;
	red = (int*)malloc(N*N*sizeof(int));
	magnetizacion = (int*)malloc(n_tira*sizeof(int));
	energia = malloc(n_tira*sizeof(float));
	correlacion = malloc(k_max* sizeof(float));
	x = malloc(n_tira * sizeof(float));
	
	FILE *fp;
	char fn[50];
	sprintf(fn,"correlacionB.txt");
	fp = fopen(fn, "w");
	
	fprintf(fp, "term %d, %d tiras de %d pasos de N*N, kps_max %d\n",termalizacion,cant_tiras,n_tira,k_max);
	fprintf(fp, "B C_mag");
	
	J=0;
	for (B=0.1; B<2; B+=0.1){//inicio loop B
		poblar(red);
		
		for (k=0; k<k_max; k++){
			*(correlacion+k) = 0;
		}//set correlacion a 0 para cada B
		
		for (m=0; m<cant_tiras; m++){//inicio loop m para promediar en tiras
			*magnetizacion = 0; //set magnetizacion en 0
			for (l = 0; l < N*N; l++){
				*magnetizacion += *(red+l);
			} //magnetizacion del primer estado
			
			*energia = 0;
			energia_interaccion(red,energia,J); //energia del primer estado
			*energia -= B * *magnetizacion; //le sumo la energia del campo
			
			flipear(red, magnetizacion, energia, B, J, n_tira, paso);
			//recuerdo que flipear avanza (paso*N*N) pasos
			
			for (l=0; l<(n_tira-termalizacion); l++){
				*(x+l) = *(magnetizacion + termalizacion + l);
			}
			
			for (k=0; k<k_max; k++){ //para cada tira corta, calculo la correlacion para todos los k
				*(correlacion+k) += func_corr(x,(n_tira-termalizacion),k); //calculo la correlacion moviendome de a N*N sitios
			}
		}//end loop m promediar en tiras
		
		for (k=0; k<k_max; k++){
			*(correlacion+k) = *(correlacion+k)/cant_tiras;
		}//promedio sobre las tiras
		
		fprintf(fp,"\n%f",B);
		for(k=0; k<k_max; k++){
			fprintf(fp, " %f",*(correlacion+k));
		}//k loop para imprimir correlacion
	}//end loop B

	free(red);
	free(magnetizacion);
	free(correlacion);
	free(energia);
	free(x);
	fclose(fp);
	clock_t toc = clock();
	printf("Duracion: %.2f segundos (%.2f minutos)\a\n", (double)(toc - tic) / CLOCKS_PER_SEC,(double)(toc - tic) / (CLOCKS_PER_SEC*60));
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

int flipear(int *red, int *magnetizacion, float *energia, float B, float J, int size, int paso){
//flipea un s_inicial a un s_final. Si la energía baja, lo acepta. Si aumenta, lo acepta con una proba P.
//guarda magnetizacion y energia cada (paso*N*N) pasos
	int si,i,j,l,sum_sj; //si spin en lugar i, sum_sj suma de vecinos
	float P, random; //prob de aceptacion, nro azar para comparar
	float ej,delta_E; //delta_E es la variacion de la energia al flipear (delta_E = 2 * J * sum sisj); ej es la energía total (luego de diezmar)
	int delta_mag = 0;

	for (j=1; j<size; j++){//loop j magnetizacion/energia
		ej=0; //energia del paso j
		for (l = 0; l < paso*N*N; l++){ //loop l iteraciones salteadas
			random = (float)rand()/(float)RAND_MAX;
			i = ((float)rand()/(float)RAND_MAX)* N*N;
			si = *(red+i);
			sum_sj = sumar_sj(red, i);
			delta_E = 2 * si *(B + J * sum_sj);
			P = exp(-delta_E); //proba de aceptar

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

int energia_interaccion(int *red, float *energia, float J){
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
	}//cierra el loop en i
	media = media/(n-k);
	media_x = media_x/(n-k);
	media_x = pow(media_x,2);	// OJO! Ahora media_x es (mean(x))**2
	media_sqx = media_sqx/(n-k);

	numerador = media - media_x;
	denominador = media_sqx - media_x;

return(numerador/denominador);
}
