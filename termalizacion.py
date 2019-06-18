import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
path = os.getcwd()

# sirve para graficar los resultados de B o J de termalizacion.txt de ising_termalizacion.c

datos = pd.read_csv(path+"/termalizacionB.txt",sep=" ",skiprows=2,header=None)
paso = eval(pd.read_csv(path+"/termalizacionB.txt",nrows=1,
			sep=" ",header=None).iloc[0][0].split("=")[1])

BJ = np.array(datos[0])
N = np.arange(0,len(datos.iloc[0,1:]),1) * paso
long = int(len(N)/2.5);

plt.figure(1)
for i,valor in enumerate(BJ):
	if i%2: # si no quiero graficar todos
		plt.plot(N[:long]/1000, abs(datos.iloc[i,1:long+1]),linewidth=0.5,label='B={}'.format(valor))
plt.xlabel(r'Iteraciones ($\times1000$)')
plt.ylabel('mag por sitio |m|')
plt.legend(loc='center right')
plt.ylim([0,1])
plt.grid()
#plt.savefig('img/terma_mag_sitioB.png',dpi=150)
plt.show()

plt.figure(2)
for i,valor in enumerate(BJ):
	if i%2: # si no quiero graficar todos
		sdev = np.array([np.std(datos.iloc[i][j:]) for j in range(len(datos.iloc[i]))])
		plt.plot(N[:long]/1000,sdev[:long],label='B={}'.format(valor))
plt.grid()
plt.xlabel(r'Iteraciones de termalización ($\times1000$)')
plt.ylabel('$\sigma_M$')
plt.xlim([0,10])
plt.legend()
plt.title('Desviacion estandar de la mag por sitio')
#plt.savefig('img/terma_std_magB.png',dpi=150)
plt.show()
