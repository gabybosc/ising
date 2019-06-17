import numpy as np
import matplotlib.pyplot as plt
from funciones import find_nearest, func_corr
#from scipy.integrate import simps, trapz
plt.ion()
import os
path = os.getcwd()

datos = np.loadtxt(path+'\\importance_sampling_deltas_0_10.txt', skiprows=2)
delta = datos[:,0]
paso_delta = np.abs(delta[0] - delta[1])
n_serie = len(datos[0,:])-1


# Aceptacion en funcion de deltas entre 0 y 10

aceptacion = np.zeros(len(delta))
for i in range(len(delta)):
    for j in range(n_serie):
        if datos[i, j] - datos[i, j+1] != 0:
            aceptacion[i] = aceptacion[i] +1
aceptacion = aceptacion*100/n_serie #asi ya esta en porcentaje
idx = np.where(aceptacion == find_nearest(aceptacion, 50))[0][0] #donde la acpetacion es 50%

plt.figure(1)
plt.plot(delta, aceptacion)
plt.plot(delta[idx],aceptacion[idx],'ro',label='$\delta$ = {}'.format(delta[idx]))
plt.xlabel('paso de exploración $\delta$')
plt.ylabel('porcentaje de aceptación')
plt.title('Aceptación de la muestra variando $\delta$ en {}'.format(paso_delta))
plt.ylim([0,100])
plt.legend()
plt.grid()
#plt.savefig('aceptacion_delta.png',dpi=150)
plt.show()


# Funcion correlacion para algunos deltas

delta_inicio = 1
delta_fin = 6
n_deltas = 10

ni = np.where(delta == find_nearest(delta, delta_inicio))[0][0]
nf = np.where(delta == find_nearest(delta, delta_fin))[0][0]

k_max = 20
ks = np.arange(0,k_max,1)
paso = 100 # longitud de las tiras sobre las que promediamos
inicio = 500 # Si queremos termalizar, sino poner 0
fin = len(datos[0,1:])

plt.figure()
for indice in np.arange(ni,nf,int(round((nf-ni)/n_deltas))): # loop para algunos deltas
    x_N = datos[indice,1:]
    # para cada delta, separar la tira de x en tiras mas cortas y hacer un promedio sobre ellas
    ck = np.zeros(len(ks))
    for j in np.arange(inicio,fin,paso): # loop para subtiras de x_N
        x = x_N[j:j+paso]
        for i,k in enumerate(ks): # inicio loop k
            corr = func_corr(x, k)
            ck[i] = ck[i] + corr
    ck_mean = ck/((fin-inicio)/paso)
    plt.plot(ks, ck_mean, label='$\delta$ = {}'.format(delta[indice]))
plt.xlabel('distancia k\n(pasos en cadena de Markov)')
plt.ylabel('correlación C(k)')
plt.title('Funcion correlación para distintos valores de $\delta$')
plt.legend()
plt.grid()
plt.savefig('correlacion_sampling.png',dpi=150)
plt.show()

# x_N = datos[idx,1:]
# # para cada delta, separar la tira de x en tiras mas cortas y hacer un promedio sobre ellas
# ck = np.zeros(len(ks))
# for j in np.arange(inicio,fin,paso): # loop para subtiras de x_N
#     x = x_N[j:j+paso]
#     for i,k in enumerate(ks): # inicio loop k
#         corr = func_corr(x, k)
#         ck[i] = ck[i] + corr
# ck_mean = ck/((fin-inicio)/paso)
# plt.plot(ks, ck_mean, label='delta = {}'.format(delta[idx]))


# Calculo de la integral (Creo que debe dar sqrt(2pi) )
datos_integral = np.loadtxt(path+'\\importance_sampling_integral.txt')
#k = 3

"""si hacemos esto pero sin diezmar los x, nos da mal, creemos que es instructivo poner el grafiquito malo para mostrar que efectivamente cambia si no consideramos la correlacion."""
# OJO! Usar los tamanios que determina el x_diezmado por k=4 (sino, las cadenas de k=1 tienen otros tamanios y no son comparables)

x_diezmado = np.array([datos_integral[4*i] for i in range(int(len(datos_integral)/4))]) #k=4

tamanios = np.linspace(1000, len(x_diezmado), 10)
tamanios = np.array([int(tamanio) for tamanio in tamanios])
termal = range(0, 900, 100)
matriz = np.zeros((len(tamanios), len(termal)))

for j,term in enumerate(termal):
	for i,N in enumerate(tamanios):
		x = x_diezmado[term:N]
		I = np.sum(x**2)/len(x) - 1
		matriz[i,j] = I
	plt.plot(tamanios,matriz[:,j],label=termal[j])
plt.xlabel('tamaño de la cadena')
plt.ylabel('$<x^2>$ - 1')
plt.ylim([-0.12, 0.12])
plt.title('Cálculo de la integral para k = {}'.format(4))
plt.legend(loc='upper right')
plt.grid()
#plt.savefig('integral_k4.png',dpi=150)
plt.show()

x_diezmado = np.array([datos_integral[i] for i in range(int(len(datos_integral)))])

for j,term in enumerate(termal):
	for i,N in enumerate(tamanios):
		x = x_diezmado[term:N]
		I = np.sum(x**2)/len(x) - 1
		matriz[i,j] = I
	plt.plot(tamanios,matriz[:,j],label=termal[j])
plt.xlabel('tamaño de la cadena')
plt.ylabel('$<x^2>$ - 1')
#plt.ylim([-0.12, 0.12])
plt.title('Cálculo de la integral para k = {}'.format(1))
plt.legend(loc='lower right')
plt.grid()
#plt.savefig('integral_k1.png',dpi=150)
plt.show()
