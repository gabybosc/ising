import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
path = os.getcwd()

# sirve para graficar los resultados de means.txt de ising_means.c

datos = pd.read_csv(path+"/meansJ.txt",sep="\t",skiprows=1,header=0)
#metadata = pd.read_csv(path+"/means.txt",nrows=1,sep=" ",header=None)

cv = datos["var_e"]/datos["temp"]**2
chi = datos["var_m"]/datos["temp"]

plt.figure(1)
plt.plot(1/datos["temp"],datos["mag"],'.')
plt.xlabel(r'temperatura inversa $\beta$')
plt.ylabel('magnetizacion por sitio $|m|$')
plt.grid()
plt.savefig('img/Jmeanmag.png',dpi=150)

plt.figure(2)
plt.plot(1/datos["temp"],chi,'.')
plt.xlabel(r'temperatura inversa $\beta$')
plt.ylabel('susceptibilidad magnetica $\chi_M$')
plt.grid()
plt.savefig('img/Jchi.png',dpi=150)

plt.figure(3)
plt.plot(1/datos["temp"],datos["en"],'.')
plt.xlabel(r'temperatura inversa $\beta$')
plt.ylabel('energia por sitio $u$')
plt.grid()
plt.savefig('img/Jmeanen.png',dpi=150)

plt.figure(4)
plt.plot(1/datos["temp"],cv,'.')
plt.xlabel(r'temperatura inversa $\beta$')
plt.ylabel('calor específico $C_V$')
plt.grid()
plt.savefig('img/Jcv.png',dpi=150)
