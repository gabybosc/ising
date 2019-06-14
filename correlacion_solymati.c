#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>



int poblar (int *red, float p, int dim);
int contornos(int *red, int dim);
int conteo(int *red, int dim);
int flipeo(int *red, int dim, float Be, float Jo);
int imprimir(int *red,int dim);
int vecinos(int *red,int dim, int i, int j);


//Función principal:
int main(int argc,char*argv[])
{ float p=0.8, Be=0.0, Jo, mm=0.0,mm2=0.0;
  int dim, i,k,j, it=1000000000,terma=1000;

  int *red;
  int *mag, *pasos;
  float *mik,*rho;
  FILE *magne;


  time_t current_time0, current_timef;
  char *c_time_string0, *c_time_stringf ;


  current_time0 = time(NULL);

  /* Convert to local time format. */
  c_time_string0 = ctime(&current_time0);

  printf("Hora de inicio: %s", c_time_string0);




  dim = 32;


  srand(time(NULL));

  red = malloc(dim*dim*sizeof(int));    //Reserva el espacio necesario para la red.
  mag = malloc (it*sizeof(int));
  pasos = malloc(100*sizeof(int));
  mik = malloc(100*sizeof(float));
  rho = malloc(100*sizeof(float));

  magne = fopen("correlacion0.43.txt", "a");
  poblar(red, p, dim);
  contornos(red,dim);
  //imprimir(red,dim);
  *(mag) = conteo(red, dim);

  for (i=1;i<it;i++)
  {
    *(mag+i)=0;
  }

  for (i=0;i<100;i++)
  {
    *(mik+i)=0.0;
  }


  //printf("%d\n",mag);
  for(Jo=0.43;Jo<0.44;Jo+=0.01)
  {
    fprintf(magne,"%f\n", Jo);
    for (i=0; i<it;i++)
      { flipeo(red, dim, Be, Jo);
        contornos(red,dim);
        if(i>terma)
        {
            *(mag+i-terma) = conteo(red,dim);
        }



        //imprimir(red,dim);
        //printf("%d\n", mag);
        /*if((i>1000))// && (i%((dim-2)*(dim-2))==0))
          {
            fprintf(magne, "%i %i\n", i, mag);
          }*/

	if(i%10000000==0)
	{
	  printf("Voy por %i por ciento.\n",i/10000000);
	}
      }

  }

  for (i=0;i<100;i++)
  {
    *(pasos+i)=i*300;
  }




for (i=0;i<it;i++)
{
  mm+=*(mag+i);
}

mm/=(it-terma);



mm=mm*mm;


//printf("%f",mm);

for(i=0;i<it;i++)
{
  mm2+=*(mag+i) * *(mag+i);
}

mm2/=(it-terma);




  for (k=0;k<100;k++)
  {
    for(j=0;j<it;j++)
    {
    *(mik+k)+= *(mag+j) * *(mag+j+k);
    }
  }



  for(k=0;k<100;k++)
  {
    *(mik+k)/=(it-terma-*(pasos+k));
  }


  /*for(k=0;k<100;k++)
  {
    *(mik+k)-=- *(mvik+k);
  }*/



for (i=0;i<100;i++)
{
  *(rho+i) = (*(mik+i) - mm)/(mm2-mm);
}


for (i=0;i<100;i++)
{
  fprintf(magne,"%i %f\n",*(pasos+i),*(rho+i));
}


//imprimir(red,dim);

  /*p=0.99;

  poblar(red, p, dim);
  contornos(red,dim);
  //imprimir(red,dim);
  mag = conteo(red, dim);
  //printf("%d\n",mag);

  for(Jo=0.31;Jo<0.6;Jo+=0.01)
  {
    fprintf(magne,"%f\n", Jo);
    for (i=0; i<it;i++)
      { flipeo(red, dim, Be, Jo);
        mag = conteo(red,dim);
        //printf("%d\n",mag);
        contornos(red,dim);
        if((i>((dim-2)*(dim-2)*100)))// && (i%((dim-2)*(dim-2))==0))
          {
            fprintf(magne, "%i %i\n", i, mag);
          }
        }
  }*/


  //printf("%i\n", mag);
  current_timef = time(NULL);

  /* Convert to local time format. */
  c_time_stringf = ctime(&current_timef);

  printf("Hora de finalizacion: %s", c_time_stringf);


  free(red);
  free(mag);
  free(pasos);
  free(mik);
  free(rho);
  fclose(magne);



  return 0;
}

//Funciones secundarias:
int poblar(int *red, float p, int dim)
{ float random;
  int i, j;

  //srand(1);

  for (i=1;i<dim-1;i++)
    { for (j=1;j<dim-1;j++)
      { *(red+i*dim+j)=0;                           //Asigna por defecto el valor <vacio>.
      random=((float)rand())/((float) RAND_MAX);    //Valor aleatorio entre 0 y 1.
      if (random<p)
        {*(red+i*dim+j)=1;                          //Asigna spin pos. si el valor aleatorio es menor.
        }
      else
        {*(red+i*dim+j)=-1;                         //Asigna spin neg. si el valor aleatorio es mayor.
        }
      }
    }

  return 0;
}

int contornos(int *red, int dim)
  { int i;

    for (i=1;i<dim-1;i++)
      { *(red+i) = *(red+(dim-2)*dim+i);        //Contorno Sup.
        *(red+(dim-1)*dim+i) = *(red+dim+i);    //Contorno Inf.
        *(red+i*dim) = *(red+(i+1)*dim-2);      //Contorno Izq.
        *(red+(i+1)*dim-1) = *(red+i*dim+1);    //Contorno Der.
      }

    return 0;
  }

int flipeo(int *red, int dim, float Be, float Jo)
  { float r;
    float *P_int;
    //float *P_ind;
    int i, j, c=0, ii, jj;



    //P_ind = malloc(2*sizeof(float));
    P_int = malloc(5*sizeof(float));

    //*(P_ind) = exp(2.0*Be);
    //*(P_ind+1) = 1.0/exp(2.0*Be);


    *(P_int) = exp(8.0*Jo);
    *(P_int+1) = exp(4.0*Jo);
    *(P_int+2) = 1;
    *(P_int+3) = exp(-4.0*Jo);
    *(P_int+4) = exp(-8.0*Jo);



    for (i=1;i<dim-1;i++)
      { for (j=1;j<dim-1;j++)
        { r = ((float)rand())/((float)RAND_MAX);
          ii = rand()%(dim-2) + 1;
          jj = rand()%(dim-2) + 1;

          if(*(P_int+vecinos(red, dim, ii, jj))>r)
            { *(red+ii*dim+jj) = - *(red+ii*dim+jj);
              if(*(red+ii*dim+jj)<0)
                {
                  c += -2;
                }
              else
                { c += 2;
                }
            }
          }
        }

    //free(P_ind);
    free(P_int);

    return 0;
  }

int conteo(int *red, int dim)
{ int i, j, c=0;

  for (i=1;i<dim-1;i++)
    { for (j=1;j<dim-1;j++)
        {
          c+=*(red+i*dim+j);
        }
    }
    return c;
}

int vecinos(int *red,int dim, int i, int j)
{ int S0,S1,S2, S3, S4, index;

  S1 = *(red+(i*dim+j-1));   //El casillero de la izquierda.
  S2 = *(red+(i*dim+j-dim)); //El casillero de arriba.
  S3 = *(red+(i*dim+j+1));    //El casillero de la derecha.
  S4 = *(red+(i*dim+j+dim));  //El casillero de la abajo.
  S0 = *(red+(i*dim+j));


  index = (S0*(S1+S2+S3+S4)+4)/2;

  return index;

}


int imprimir(int *red, int dim)         //Imprime una fila debajo de la otra.
{ int i,j;

  for (i=0;i<dim;i++)
    {for (j=0;j<dim;j++)
      {
      printf("%02d ", *(red+dim*i+j));
      }
    printf("\n");
    }

  return 0;
}
