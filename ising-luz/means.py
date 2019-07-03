import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from funciones import find_nearest
import os
path = os.getcwd()
plt.ion()
# sirve para graficar los resultados de means.txt de ising_means.c

# for N in [8,16,64,128]:
N = 64
datos = pd.read_csv(path+f"/means_N128.txt",sep="\t",skiprows=1,header=0)
beta = 1/datos['temp']
#metadata = pd.read_csv(path+"/means.txt",nrows=1,sep=" ",header=None)
fin = np.where(beta == find_nearest(beta, 0.6))[0][0]
plt.figure()
plt.plot(beta[fin:],datos["Mag"][fin:], '.')
plt.axvline(0.44, c = 'r')
plt.xlabel(r'Temperatura inversa $\beta$')
plt.title(f'Magnetización por sitio para L = {N}')
plt.grid()
plt.savefig(f'img/mag_L{N}.png')

plt.figure()
plt.plot(beta[fin:],datos["En"][fin:], '.')
plt.xlabel(r'Temperatura inversa $\beta$')
plt.title(f'Energía por sitio para L = {N}')
plt.grid()
plt.axvline(0.44, c = 'r')
plt.savefig(f'img/en_L{N}.png')
# plt.show()
