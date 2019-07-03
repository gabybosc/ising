import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
path = os.getcwd()
plt.ion()
# sirve para graficar los resultados de B o J de termalizacion.txt de ising_termalizacion.c

datos = pd.read_csv(path+"/termalizacion_antiferro.txt",sep="\t",skiprows=2,header=None)
# paso = int(pd.read_csv(path+"/termalizacion.txt",nrows=1,
# 			sep="\t",header=None).iloc[0][0].split("=")[1])

paso = 64**2
BJ = np.array(datos[0])
N = np.arange(0,len(datos.iloc[0,1:]),1) * paso

plt.figure(1)
for i,valor in enumerate(BJ):
	# if i%2:
	plt.plot(N/1000,abs(datos.iloc[i,1:]),label=f'B={valor}')
plt.xlabel(r'Iteraciones ($\times1000$)')
plt.ylabel(r'Magnetización por sitio |$m$|')
plt.legend()
plt.title('Magnetización por sitio para distintos B')
plt.ylim([0,1])
plt.grid()
#plt.savefig('terma_mag_sitio.png',dpi=150)

plt.figure(2)
for i,valor in enumerate(BJ):
	# if i%2: # si no quiero graficar todos
	sdev = np.array([np.std(datos.iloc[i][j:]) for j in range(len(datos.iloc[i]))])
	plt.plot(N/1000,sdev[:-1],label=f'B = {valor}')
plt.grid()
plt.xlabel(r'Iteraciones ($\times1000$)')
plt.ylabel('$\sigma_M$')
plt.legend()
plt.title('Desviación estándar de la magnetización por sitio para distintos B')
#plt.savefig('terma_std_mag.png',dpi=150)
