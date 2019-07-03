import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
path = os.getcwd()

# sirve para graficar los resultados de correlacion*.txt de ising_corr*.c

datos = pd.read_csv(path+"/antiferro_corr.txt",sep=" ",skiprows=2,header=None)
#datos = pd.read_csv(path+"/correlacionB.txt",sep=" ",skiprows=2,header=None)
#metadata = pd.read_csv(path+"/correlacionB.txt",nrows=1,sep=" ",header=None)

BJ = np.array(datos[0])
N = np.arange(0,len(datos.iloc[0,1:]),1024) # el paso es de N*N con N=32 (ver metadata)

plt.figure(1)
for i,valor in enumerate(BJ):
    #if i%2: # si no quiero graficar todos
	plt.plot(abs(datos.iloc[i,1:]),label=valor)
plt.xlabel(r'Iteraciones ($\times1000$)')
plt.ylabel('mag por sitio |m|')
plt.legend()
#plt.ylim([0,1])
plt.grid()
#plt.savefig('terma_mag_sitio.png',dpi=150)
plt.show()
