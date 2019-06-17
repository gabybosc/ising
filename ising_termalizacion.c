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

//------------MAIN-------------
int main(){
	// devuelve magnetizacion por sitio para cada campo externo B o interaccion J
	int size = 1000; // muestras de magnetizacion y energia
	int paso = 1; // paso*N*N pasos entre iteraciones guardadas
	
	float B,J;
	
	srand(time(NULL));
	int *red, *magnetizacion, j;
	float *energia;
	red = (int*)malloc(N*N*sizeof(int));
	magnetizacion = (int*)malloc(size*sizeof(int));
	energia = malloc(size*sizeof(float));
	
	FILE *fp;
	char fn[30];
	sprintf(fn,"termalizacion.txt");
	fp = fopen(fn, "w");
	
	fprintf(fp,"N%d;paso_iteracion=%d\n",N,paso*N*N);
	fprintf(fp,"BJ m");
	
	/*
	//loop en B fijando J
	J=0;
	for (B = 0.2; B<3; B+=0.2){
		poblar(red);

		*magnetizacion = 0;
		for (j = 0; j < N*N; j++){
			*magnetizacion += *(red+j);
		}//magnetizacion del primer estado
		
		*energia = 0;
		energia_interaccion(red,energia,J); //energia del primer estado
		*energia -= B * *magnetizacion; //le sumo la energia del campo
		
		flipear(red, magnetizacion, energia, B, J, size, paso);
		fprintf(fp,"\n%f",B);
		for(j = 0; j<size; j++){
			fprintf(fp," %f", (float)*(magnetizacion+j)/(float)(N*N));
		}
	}
	*/
	
	//loop en J fijando B
	B=0;
	for (J=0.1; J<0.61; J+=0.1){
		poblar(red);
		//para J puede convenir usar la red del paso anterior

		*magnetizacion = 0;
		for (j = 0; j < N*N; j++){
			*magnetizacion += *(red+j);
		}//magnetizacion del primer estado

		*energia = 0;
		energia_interaccion(red,energia,J); //energia del primer estado
		*energia -= B * *magnetizacion; //le sumo la energia del campo
		
		flipear(red, magnetizacion, energia, B, J, size, paso);
		fprintf(fp,"\n%f",J);
		for(j = 0; j<size; j++){
			fprintf(fp," %f", (float)*(magnetizacion+j)/(float)(N*N));
		}
	}
	
	free(red);
	free(magnetizacion);
	free(energia);
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
//flipea un s_inicial a un s_final. Si la energía baja, lo acepta. Si aumenta, lo acepta con una proba P
//guarda magnetizacion y energia cada (paso*N*N) pasos
	int si,i,j,l,sum_sj; //si spin en lugar i, sum_sj suma de vecinos
	float P, random; //prob de aceptacion, nro azar para comparar
	float ej,delta_E; //energia ej que se guarda en el vector, delta energia de cada flip
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
	//calculo de energia de interaccion del primer estado
	float hamiltoniano = 0;
	int i;

	for(i = 0; i < N*N; i++){
		hamiltoniano += *(red+i) * sumar_sj(red, i);
	}
	*energia = - J * hamiltoniano / 2;
	return(0);
}

