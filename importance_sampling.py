import numpy as np
import matplotlib.pyplot as plt
from funciones import find_nearest, num_func_corr
plt.ion()
"""
veamos la diferencia entre dos puntos consecutivos: si es nula, es que no aceptó, si es no nula es que sí aceptó.
"""

datos = np.loadtxt('importance_sampling_deltas_0_3.txt', skiprows=2)
delta = datos[:,0]
paso = np.abs(delta[0] - delta[1])
aceptacion = np.zeros(len(delta))

for i in range(len(delta)):
    for j in range(len(datos[i,:])-1):
        if datos[i, j] - datos[i, j+1] != 0:
            aceptacion[i] = aceptacion[i] +1

aceptacion = aceptacion /100 #asi ya esta en porcentaje
idx = np.where(aceptacion == find_nearest(aceptacion, 50))[0][0] #donde la acpetacion es 50%

delta_critico = delta[idx]

k_max = 20
ks = np.arange(0,k_max,1)

for indice in np.arange(100,1000,100): # loop deltas
    x_N = datos[indice,:]
    # para cada delta, separar la tira de x en tiras mas cortas y hacer un promedio sobre ellas
    paso = 100
    inicio = 500 # Si queremos termalizar, sino poner 0
    fin = len(datos[0,1:])
    ck = np.zeros(len(ks))
    for j in np.arange(inicio,fin,paso):
        x = x_N[j:j+paso]
        for i,k in enumerate(ks): # inicio loop k
            corr = num_func_corr(x, k) #el primer termino del numerador de la función correlacion
            numerador = corr - np.mean(x)**2 #corregir el mean
            denominador = np.std(x) #corregir la std (mejro que haga todo directamene en la funcion)
            ck[i] = ck[i] + numerador/denominador

    ck_mean = ck/((fin-inicio)/paso)
    plt.plot(ks, ck_mean, label='delta = {}'.format(delta[indice]))
plt.xlabel('k')
plt.ylabel('C(k)')
plt.title('Correlacion')
plt.legend()
# plt.figure(1)
# plt.plot(delta, aceptacion)
# plt.xlabel('delta')
# plt.ylabel('porcentaje de acpetacion')
# plt.title('Aceptación para un paso entre deltas de {}'.format(paso))
